#include "KinemSeek.h"

physics::SteeringOutput AI::KinemSeek::getSteering()
{
	physics::SteeringOutput steering;

	ofVec2f dir = (mTarget - mCharacter.mPosition).normalize(); // get vel dir
	steering.mLinear = dir * mMaxSpeed;

	return steering;
}
