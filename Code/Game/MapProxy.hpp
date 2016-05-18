#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/TheWorld.hpp"

enum eMovementProperties;

namespace MapProxy {

	float ComputeLogalG(IntVector2 activeNodePos, IntVector2 pos, int movementProperties);
	float ComputeHueristic(IntVector2 pos, IntVector2 goal);
	std::vector<IntVector2> FindValidAdjacentPositions(IntVector2 pos, int movementProperties);

}