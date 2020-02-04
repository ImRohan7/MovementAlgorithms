#include "Kinematic.h"

#define MaxVel 4

namespace physics {

	void Kinematic::update(SteeringOutput iSteering, float iElapsedTime)
	{
		
		mPosition += mVelocity * iElapsedTime*50;
		mOrientation += mRotaionvel * iElapsedTime;

		mVelocity += iSteering.mLinear * iElapsedTime;
		if (mVelocity.x > MaxVel)	mVelocity.x = MaxVel;
		if (mVelocity.x < -MaxVel)	mVelocity.x = -MaxVel;
		if (mVelocity.y > MaxVel)	mVelocity.y = MaxVel;
		if (mVelocity.y < -MaxVel)	mVelocity.y = -MaxVel;

		mOrientation += iSteering.mAngular * iElapsedTime;
	}


	// returns new orientaion based on velocity
	float Kinematic::getNewOrientation(ofVec2f ivel, float ior)
	{
		return ivel.length() > 0 ? atan2(ivel.y, ivel.x) : ior;
	}

	void Kinematic::updateOrientation()
	{
		mOrientation =  mVelocity.length() > 0 ? atan2(mVelocity.y, mVelocity.x) : mOrientation;
	}
}