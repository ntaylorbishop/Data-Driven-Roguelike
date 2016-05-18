#include "Game/MapProxy.hpp"
#include "Game/NPC.hpp"

float MapProxy::ComputeLogalG(IntVector2 activeNodePos, IntVector2 pos, int movementProperties) {
	IntVector2 diff = IntVector2(abs(pos.x - activeNodePos.x), abs(pos.y - activeNodePos.y));

	float g = 1.f;

	if (diff.x != 0 && diff.y != 0) { //Diagonal
		g += 0.4f;
	}

	Tile* tileToCheck = g_theWorld->m_activeMap->GetTileAtLocation(pos);

	if (BitUtils::IsBitSet(movementProperties, SLOWED_BY_LAVA)) {
		if (tileToCheck && tileToCheck->GetCurrentTileType() == TILE_LAVA) {
			g += 0.5f;
		}
	}
	if (BitUtils::IsBitSet(movementProperties, SLOWED_BY_WATER)) {
		if (tileToCheck && tileToCheck->GetCurrentTileType() == TILE_WATER) {
			g += 0.5f;
		}
	}

	return g;
}
float MapProxy::ComputeHueristic(IntVector2 pos, IntVector2 goal) {
	return (float)abs(goal.x - pos.x) + (float)abs(goal.y - pos.y);
}
std::vector<IntVector2> MapProxy::FindValidAdjacentPositions(IntVector2 pos, int movementProperties) {
	ASSERT_OR_DIE(g_theWorld->m_activeMap != nullptr, "ERROR: NO MAP LOADED IN THE WORLD TO PATHFIND ON");
	
	std::vector<IntVector2> validPositions;
	std::vector<IntVector2> possiblePositions;
	validPositions.reserve(8);
	possiblePositions.reserve(8);

	possiblePositions.push_back(pos + IntVector2( 0,  1)); //up
	possiblePositions.push_back(pos + IntVector2( 1,  1)); //up right
	possiblePositions.push_back(pos + IntVector2( 1,  0)); //right
	possiblePositions.push_back(pos + IntVector2( 1, -1)); //down right
	possiblePositions.push_back(pos + IntVector2( 0, -1)); //down
	possiblePositions.push_back(pos + IntVector2(-1, -1)); //down left
	possiblePositions.push_back(pos + IntVector2(-1,  0)); //left
	possiblePositions.push_back(pos + IntVector2(-1,  1)); //up left


	for (unsigned int i = 0; i < possiblePositions.size(); i++) {
		Tile* currTile = nullptr;

		currTile = g_theWorld->m_activeMap->GetTileAtLocation(possiblePositions[i]);

		if (nullptr != currTile) {
			//Gather blockedByProperties
			int blockedByProperties = 0;
			if (BitUtils::IsBitSet(movementProperties, BLOCKED_BY_LAVA)) {
				blockedByProperties |= BLOCKED_BY_LAVA;
			}
			if (BitUtils::IsBitSet(movementProperties, BLOCKED_BY_WATER)) {
				blockedByProperties |= BLOCKED_BY_WATER;
			}
			if (BitUtils::IsBitSet(movementProperties, BLOCKED_BY_WALLS)) {
				blockedByProperties |= BLOCKED_BY_WALLS;
			}
			if (BitUtils::IsBitSet(movementProperties, BLOCKED_BY_AGENTS)) {
				blockedByProperties |= BLOCKED_BY_AGENTS;
			}

			int tileProperties = 0;
			if (currTile->GetCurrentTileType() == TILE_LAVA) {
				tileProperties |= IS_LAVA;
			}
			if (currTile->GetCurrentTileType() == TILE_STONE) {
				tileProperties |= IS_WALL;
			}
			if (currTile->GetCurrentTileType() == TILE_WATER) {
				tileProperties |= IS_WATER;
			}
			if (currTile->GetOccupyingAgent()) {
				tileProperties |= CONTAINS_AGENT;
			}

			bool isValid = (tileProperties & blockedByProperties) != 0;

			if (!isValid) {
				validPositions.push_back(possiblePositions[i]);
			}
		}
	}

	return validPositions;
}