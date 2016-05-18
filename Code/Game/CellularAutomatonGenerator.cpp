#include "Game/CellularAutomatonGenerator.hpp"

GeneratorRegistration CellularAutomatonGenerator::s_CAGRegistration("CellularAutomata", &CellularAutomatonGenerator::CreateGenerator);

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
CellularAutomatonGenerator::CellularAutomatonGenerator(EnvironmentGeneratorData* genData) 
	: Generator(genData)
{ }

STATIC Generator* CellularAutomatonGenerator::CreateGenerator(EnvironmentGeneratorData* genData) {
	return new CellularAutomatonGenerator(genData);
}

//---------------------------------------------------------------------------------------------------------------------------
//GENERATION
//---------------------------------------------------------------------------------------------------------------------------
Map* CellularAutomatonGenerator::GenerateMap(const IntVector2& size) {
	//Instantiate map and make it 60% air and 40% stone
	//Air is alive

	srand((uint)time(NULL));

	Map* map = new Map(size);

	for (int x = 0; x < size.x - 1; x++) {
		for (int y = 0; y < size.y - 1; y++) {
			IntVector2 loc = IntVector2(x, y);
			Tile* currTile = map->GetTileAtLocation(IntVector2(x, y));

			int whichToPick = rand() % 10 + 1;

			if (whichToPick <= 6) {
				//Is Air
				currTile->SetCurrentTileType(TILE_GRASS);
				currTile->SetLocation(loc);
			}
			else {
				//Is Stone
				currTile->SetCurrentTileType(TILE_STONE);
				currTile->SetLocation(loc);
			}
		}
	}

	return map;
}
bool CellularAutomatonGenerator::GenerateStep(Map* map, int& currentStepNumber) {
	UNREFERENCED_PARAMETER(currentStepNumber);

	IntVector2 mapSize = map->GetSize();

	const int amountOfIsolation = 1;
	const int amountToThrive = 2;
	const int amountToStarve = 5;

	for (int x = 0; x < mapSize.x - 1; x++) {
		for (int y = 0; y < mapSize.y - 1; y++) {
			Tile* currTile = map->GetTileAtLocation(IntVector2(x, y));

			int numSurrounding = map->GetNumberOfTilesSurroundingOfType(IntVector2(x, y), TILE_STONE, 2);

			if (numSurrounding == amountOfIsolation) {
				currTile->SetFutureTileType(TILE_GRASS);
			}
			else if (numSurrounding > amountToThrive && numSurrounding < amountToStarve) {
				currTile->SetFutureTileType(TILE_STONE);
			}
			else if (numSurrounding >= amountToStarve) {
				currTile->SetFutureTileType(TILE_GRASS);
			}
		}
	}

	//Update states
	for (int x = 0; x < mapSize.x - 1; x++) {
		for (int y = 0; y < mapSize.y - 1; y++) {
			Tile* currTile = map->GetTileAtLocation(IntVector2(x, y));
			currTile->SetCurrentTileType(currTile->GetFutureTileType());
		}
	}

	return true;
}

void CellularAutomatonGenerator::GenerateAll(Map* map, int numSteps) {
	int currStepNumber = 0;

	for (int i = 0; i < numSteps; i++) {
		GenerateStep(map, currStepNumber);
	}
}