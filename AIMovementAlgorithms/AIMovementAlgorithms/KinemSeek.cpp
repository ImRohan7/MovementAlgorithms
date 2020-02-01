#include "KinemSeek.h"

// for no arrival
physics::SteeringOutput AI::KinemSeek::getSteering()
{
	physics::SteeringOutput steering;

	ofVec2f dir = (mTarget.mPosition - mCharacter.mPosition).normalize(); // get vel dir
	steering.mLinear = dir * mMaxSpeed;

	return steering;
}

// with arrive
physics::SteeringOutput AI::KinemSeek::getSteeringForArrival()
{
	physics::SteeringOutput steering;

	ofVec2f dir = mTarget.mPosition - mCharacter.mPosition; // get vel dir
	float distance = dir.length();
	float targetSpeed = 0.0f;
	ofVec2f targetVel;
	// if we already reached
	if (distance < mTargetRad)
		return steering;

	// if outside slow rad then we go max velocity
	if (distance > mSlowRad)
		targetSpeed = mMaxSpeed;

	// else we scale the speed with the distance remaining
	else
		targetSpeed = mMaxSpeed * (distance / mSlowRad);

	//
	targetVel = dir.normalize();
	targetVel *= targetSpeed;

	// get acceleration dir
	steering.mLinear = targetVel - mCharacter.mVelocity;
	steering.mLinear /= mTimeTotarget;

	// clamp acceleration
	if (steering.mLinear.length() > mMaxAccel)
	{
		steering.mLinear.normalize();
		steering.mLinear *= mMaxAccel;
	}
	steering.mAngular = 0;
	return steering;
}