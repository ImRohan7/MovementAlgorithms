#pragma once
#include "ofMath.h"
#include "ofVectorMath.h"



namespace physics {

	/* Steering behaviors operate with these kinematic data. They return accelerations that will
	  change the velocities of a character in order to move them around the level. Their output is a set
		of accelerations:	*/

	struct SteeringOutput {

	public:
		ofVec2f mLinear; // linear acceleration
		float mAngular;	// angluar acceleration
	};

	struct Kinematic
	{

		void update(SteeringOutput iSteering, float iElapsedTime);
		float getNewOrientation(ofVec2f ivel, float iCurOrientation);

	public:
		ofVec2f mPosition;
		ofVec2f mVelocity;
		float mOrientation;
		float mRotaionvel; // rot velocity
	};

	
}

