#pragma once
#include "src/Kinematic.h"

namespace AISystem {
	enum Algo
	{
		Basic_Kinematic,
		SeekArrive,
		SeekArrive2,
		WanderSteering,
		Flocking
	};


	// retunrs steering individual
	physics::SteeringOutput getSteeringForFlocking()
	{

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
