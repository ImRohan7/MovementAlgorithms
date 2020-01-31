#pragma once
#include "src/Kinematic.h"

namespace AI {

	class KinemSeek
	{
	public:

		KinemSeek() {};

		KinemSeek(physics::Kinematic c, ofVec2f tar, float sp)
			: mCharacter(c), mTarget(tar), mMaxSpeed(sp){}

		// returns steering based on player and target
		physics::SteeringOutput getSteering();

		physics::Kinematic mCharacter;
		ofVec2f mTarget;
		float mMaxSpeed;
	};
}

