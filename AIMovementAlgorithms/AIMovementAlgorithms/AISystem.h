#pragma once
#include "src/Kinematic.h"
#include <vector>

using namespace std;
namespace AISystem {
	enum Algo
	{
		Basic_Kinematic,
		SeekArrive,
		SeekArrive2,
		WanderSteering,
		Flocking
	};

	static ofVec2f sCom; // center of mass
	static ofVec2f sComVel; // center of vel

	static float sMaxSpeed = 15.0f;

	static void calcCom(
		physics::Kinematic leader, vector<physics::Kinematic> foll )
	{
		ofVec2f com;
		float total = 0;
		for (physics::Kinematic k : foll)
		{
			com += (k.mPosition * k.mWeight);
			total += k.mWeight;
		}
		com += (leader.mPosition*leader.mWeight);
		total += leader.mWeight;
		com /= total;
		sCom = com;
	}

	static void calcCovel(
		physics::Kinematic leader, vector<physics::Kinematic> foll)
	{
		ofVec2f covel;
		float total = 0;
		for (physics::Kinematic k : foll)
		{
			covel += (k.mVelocity * k.mWeight);
			total += k.mWeight;
		}
		covel += (leader.mVelocity*leader.mWeight);
		total += leader.mWeight;
		covel /= total;
		sComVel = covel;
	}

	// retunrs steering individual
	static physics::SteeringOutput getSteeringForFlocking(
	physics::Kinematic leader, vector<physics::Kinematic> foll, int i	)
	{
		physics::SteeringOutput steer;
		physics::Kinematic chr = foll[i];
		// seek to center of mass + look where going
		// com
		ofVec2f seekVel;
		seekVel = sCom - chr.mPosition;
		seekVel = seekVel.normalize();
		seekVel *= sMaxSpeed;
		// angle
		steer.mAngular = chr.getNewOrientation(seekVel, chr.mOrientation);
		
		ofVec2f separtVel;
		// separate from others
		for (int j=0; j<foll.size(); j++)
		{
			if (j != i)
			{
				if (foll[j].mPosition.distance(chr.mPosition) < chr.mSepRadius)
				{
					ofVec2f a = (chr.mPosition - foll[j].mPosition);
					a = a.normalize();
					separtVel += a * 12.0f;
					steer.mLinear = separtVel;
					//return steer;
				}
			}
		}

		// velocity match
		ofVec2f matchVel = sComVel - chr.mVelocity;

		steer.mLinear = seekVel*5 + separtVel * 10 + matchVel * 100;
	
		return steer;
	}


	/*
	switch (sAlgo)
	{
	case AISystem::Algo::Kinematic:

		break;
	case AISystem::Algo::SeekArrive:
	case AISystem::Algo::SeekArrive2:

		break;
	case AISystem::Algo::WanderSteering:

		break;
	case AISystem::Algo::Flocking:

		break;

	default:
		std::cout << "Unknown case passed";
		break;
	}
	*/
}
