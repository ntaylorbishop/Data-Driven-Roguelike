#include "Game/DungeonGenerator.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
DungeonGenerator::DungeonGenerator(EnvironmentGeneratorData* genData) 
	: Generator(genData)
{ }

//---------------------------------------------------------------------------------------------------------------------------
//GENERATION
//---------------------------------------------------------------------------------------------------------------------------
Map* DungeonGenerator::GenerateMap(const IntVector2& size) {
	InitializeRandomSeedForRand();
	Map* map = new Map(size, TILE_STONE);

	IntVector2 center = size / 2;

	IntVector2 offset = IntVector2(RandInt(5, 10), RandInt(5, 10));

	Room roomToAdd = AddRoom(map);
	m_rooms.push_back(roomToAdd);

	return map;
}

//---------------------------------------------------------------------------------------------------------------------------
bool DungeonGenerator::GenerateStep(Map* map, int& currentStepNumber) {
	UNREFERENCED_PARAMETER(currentStepNumber);

	Room roomToAdd = AddRoom(map);
	if (roomToAdd.mins != IntVector2(0, 0) && roomToAdd.maxs != IntVector2(0, 0)) {
		m_rooms.push_back(roomToAdd);
		ConnectRoomToDungeon(map, roomToAdd);
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
void DungeonGenerator::GenerateAll(Map* map, int numSteps) {
	int currStepNumber = 0;

	for (int i = 0; i < numSteps; i++) {
		GenerateStep(map, currStepNumber);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
GeneratorRegistration DungeonGenerator::s_dungeonRegistration("Dungeon", &DungeonGenerator::CreateGenerator);

STATIC Generator* DungeonGenerator::CreateGenerator(EnvironmentGeneratorData* genData) {
	return new DungeonGenerator(genData);
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
Room DungeonGenerator::AddRoom(Map* map) {
	IntVector2 mapSize = map->GetSize();

	int howManyTimesRun = 0;
	bool b = true;
	while (b) {
		IntVector2 center = IntVector2(RandInt(3, mapSize.x - 2), RandInt(6, mapSize.y - 2));
		IntVector2 offset = IntVector2(RandInt(2, 4), RandInt(2, 4));
		Room roomToAdd = Room(center - offset, center + offset);
		
		if (!DoesRoomIntersect(roomToAdd)) {
			BlockOutRoomOnMap(map, roomToAdd);
			return roomToAdd;
		}
		else {
			howManyTimesRun++;
			//DebuggerPrintf("%i", howManyTimesRun);
			
			if (howManyTimesRun >= 1000) {
				//ERROR_AND_DIE("C1099: Dungeon generator add room has been trying to add a room for too many cycles");
				return Room(IntVector2(0, 0), IntVector2(0, 0));
			}
		}
	}

	return Room(IntVector2(0, 0), IntVector2(0, 0));
}
//---------------------------------------------------------------------------------------------------------------------------
void DungeonGenerator::ConnectRoomToDungeon(Map* map, const AABB2Int& newRoom) {

	//Find room to connect to
	int width = RandInt(1, 2);
	IntVector2 newRoomCtr = ((newRoom.maxs - newRoom.mins) / 2) + newRoom.mins;
	int randIdx;
	if (m_rooms.size() == 2) {
		randIdx = 0;
		//randIdx = RandInt(0, m_rooms.size() - 1);
	}
	else if(m_rooms.size() <= 1) {
		return;
	}
	else {
		randIdx = RandInt(0, m_rooms.size() - 2);
	}
	Room connectingRoom = m_rooms[randIdx];
	IntVector2 connectingRoomCtr = ((connectingRoom.maxs - connectingRoom.mins) / 2) + connectingRoom.mins;

	IntVector2 connectingPoint;

	connectingPoint = newRoomCtr;

	IntVector2 currPoint = connectingPoint;

	int timesRan = 0;
	while (currPoint != connectingRoomCtr) {
		if (currPoint.x < connectingRoomCtr.x) {
			if (!IsPointInSquare(currPoint, connectingRoom) && !IsPointInSquare(currPoint, newRoom)) {
				for (int i = 1; i < width; i++) {
					Tile* t = map->GetTileAtLocation(currPoint + IntVector2(0, -1 * i));
					if(nullptr != t)
						t->SetCurrentTileType(TILE_GRASS);

					t = map->GetTileAtLocation(currPoint + IntVector2(0, 1 * i));
					if (nullptr != t)
						t->SetCurrentTileType(TILE_GRASS);
				}
/*
				IntVector2 topLoc1 = currPoint + IntVector2(0, -1);
				IntVector2 topLoc1 = currPoint + IntVector2(0, -1);
				IntVector2 topLoc1 = currPoint + IntVector2(0, -1);
				IntVector2 botLoc = currPoint + IntVector2(0, 1);*/

				map->GetTileAtLocation(currPoint)->SetCurrentTileType(TILE_GRASS);
			}
			currPoint.x += 1;
		}
		else if (currPoint.x > connectingRoomCtr.x) {
			if (!IsPointInSquare(currPoint, connectingRoom) && !IsPointInSquare(currPoint, newRoom)) {
				for (int i = 1; i < width; i++) {
					Tile* t = map->GetTileAtLocation(currPoint + IntVector2(0, -1 * i));
					if (nullptr != t)
						t->SetCurrentTileType(TILE_GRASS);

					t = map->GetTileAtLocation(currPoint + IntVector2(0, 1 * i));
					if (nullptr != t)
						t->SetCurrentTileType(TILE_GRASS);
				}

				map->GetTileAtLocation(currPoint)->SetCurrentTileType(TILE_GRASS);
			}
			currPoint.x -= 1;
		}
		else if (currPoint.x == connectingRoomCtr.x) {
			if (currPoint.y < connectingRoomCtr.y) {
				if (!IsPointInSquare(currPoint, connectingRoom) && !IsPointInSquare(currPoint, newRoom)) {
					for (int i = 1; i < width; i++) {
						Tile* t = map->GetTileAtLocation(currPoint + IntVector2(-1 * i, 0));
						if (nullptr != t)
							t->SetCurrentTileType(TILE_GRASS);

						t = map->GetTileAtLocation(currPoint + IntVector2(1 * i, 0));
						if (nullptr != t)
							t->SetCurrentTileType(TILE_GRASS);
					}

					map->GetTileAtLocation(currPoint)->SetCurrentTileType(TILE_GRASS);
				}
				currPoint.y += 1;
			}
			else if (currPoint.y > connectingRoomCtr.y) {
				if (!IsPointInSquare(currPoint, connectingRoom) && !IsPointInSquare(currPoint, newRoom)) {
					for (int i = 1; i < width; i++) {
						Tile* t = map->GetTileAtLocation(currPoint + IntVector2(-1 * i, 0));
						if (nullptr != t)
							t->SetCurrentTileType(TILE_GRASS);

						t = map->GetTileAtLocation(currPoint + IntVector2(1 * i, 0));
						if (nullptr != t)
							t->SetCurrentTileType(TILE_GRASS);
					}

					map->GetTileAtLocation(currPoint)->SetCurrentTileType(TILE_GRASS);
				}
				currPoint.y -= 1;
			}
			else if (currPoint.y == connectingRoomCtr.y) {
				map->GetTileAtLocation(currPoint)->SetCurrentTileType(TILE_GRASS);
				break;
			}
		}

		timesRan++;

// 		if (timesRan > 10000) {
// 
// 		}
	}

	map->GetTileAtLocation(connectingPoint)->SetCurrentTileType(TILE_GRASS);

	//IntVector2 currPoint = connectingPoint;
	//CleanUpRooms(map);
}

//---------------------------------------------------------------------------------------------------------------------------
bool DungeonGenerator::DoesRoomIntersect(const Room& room) {
	bool intersects = false;
	for (unsigned int i = 0; i < m_rooms.size(); i++) {
		Room currRoom = m_rooms[i];

		int ax1 = room.mins.x;
		int ay1 = room.mins.y;
		int ax2 = room.maxs.x;
		int ay2 = room.maxs.y;

		int bx1 = currRoom.mins.x;
		int by1 = currRoom.mins.y;
		int bx2 = currRoom.maxs.x;
		int by2 = currRoom.maxs.y;

		bool first  = ax1 <= bx2;
		bool second = ax2 >= bx1;
		bool third  = ay1 <= by2;
		bool fourth = ay2 >= by1;

		if (first && second && third && fourth) {
			intersects = true;
		}
	}

	return intersects;
}

//---------------------------------------------------------------------------------------------------------------------------
void DungeonGenerator::BlockOutRoomOnMap(Map* map, const Room& roomToAdd) {
	for (int x = roomToAdd.mins.x - 1; x < roomToAdd.maxs.x + 1; x++) {
		for (int y = roomToAdd.mins.y - 1; y < roomToAdd.maxs.y + 1; y++) {
			IntVector2 currLoc = IntVector2(x, y);

			Tile* t = map->GetTileAtLocation(currLoc);
			if (nullptr != t) {
				t->SetCurrentTileType(TILE_GRASS);
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
/*
void DungeonGenerator::CleanUpRooms(Map* map) {
	for (int i = 0; i < m_rooms.size(); i++) {
		for (int x = m_rooms[i].mins.x + 1; x < m_rooms[i].maxs.x - 1; x++) {
			for (int y = m_rooms[i].mins.y + 1; y < m_rooms[i].maxs.y - 1; y++) {
				IntVector2 currLoc = IntVector2(x, y);

				map->GetTileAtLocation(currLoc)->SetCurrentTileType(TILE_AIR);
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void DungeonGenerator::CleanUpHallways(Map* map) {
	for (unsigned int i = 0; i < m_hallwayLocs.size(); i++) {
		map->GetTileAtLocation(m_hallwayLocs[i])->SetCurrentTileType(TILE_AIR);
	}
}*/