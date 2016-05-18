#pragma once

#include <stdlib.h>

#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/Map.hpp"
#include "Game/Generator.hpp"

/*
struct Room {
	Room(IntVector2 mins, IntVector2 maxs) : mins(mins), maxs(maxs) {}

	IntVector2 mins;
	IntVector2 maxs;

	std::vector<IntVector2> 
}*/

typedef AABB2Int Room;

class DungeonGenerator : public Generator {
public:
	//STRUCTORS
	DungeonGenerator(EnvironmentGeneratorData* genData);

	//GENERATION
	Map* GenerateMap(const IntVector2& size);
	bool GenerateStep(Map* map, int& currentStepNumber);
	void GenerateAll(Map* map, int numSteps);

	//STATIC
	static GeneratorRegistration s_dungeonRegistration;
	static Generator* CreateGenerator(EnvironmentGeneratorData* genData);

private:
	//PRIVATE MEMBER FUNCTIONS
	Room AddRoom(Map* map);
	void ConnectRoomToDungeon(Map* map, const Room& newRoom);
	
	bool DoesRoomIntersect(const Room& room);
	void BlockOutRoomOnMap(Map* map, const Room& roomToAdd);

	//void CleanUpRooms(Map* map);
	//void CleanUpHallways(Map* map);

	std::vector<IntVector2> m_hallwayLocs;
	std::vector<Room> m_rooms;
};