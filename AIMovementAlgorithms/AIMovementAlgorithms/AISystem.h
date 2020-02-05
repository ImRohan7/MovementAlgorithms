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

	static ofVec2f sCom;
	static float sMaxSpeed = 8.0f;

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

	// retunrs steering individual
	static physics::SteeringOutput getSteeringForFlocking(
	physics::Kinematic leader, vector<physics::Kinematic> foll, int i	)
	{
		physics::SteeringOutput steer;
		physics::Kinematic chr = foll[i];
		// seek to center of mass + look where going
		// com
		ofVec2f velA;
		velA = sCom - chr.mPosition;
		velA = velA.normalize();
		velA *= sMaxSpeed;
		// angle
		steer.mAngular = chr.getNewOrientation(velA, chr.mOrientation);
		
		ofVec2f velB;
		// separate from others
		for (int j=0; j<foll.size(); j++)
		{
			if (j != i)
			{
				if (foll[j].mPosition.distance(chr.mPosition) < chr.mSepRadius)
				{
					ofVec2f a = (chr.mPosition - foll[j].mPosition);
					a = a.normalize();
					velB += a * 12.0f;
					steer.mLinear = velB;
					return steer;
				}
			}
		}

		// velocity match
		ofVec2f velC = leader.mVelocity-chr.mVelocity;

		steer.mLinear = velA + velB + velC;
	
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
