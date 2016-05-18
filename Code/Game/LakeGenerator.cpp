#include "Game/LakeGenerator.hpp"

GeneratorRegistration LakeGenerator::s_LakeRegistration("Lake", &LakeGenerator::CreateGenerator);

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
LakeGenerator::LakeGenerator(EnvironmentGeneratorData* genData)
	: Generator(genData)
	, m_currWaterLevel(-0.5f)
	, m_perlinSeed(0)
	, m_locSeed(0.f)
	, m_tileType(TILE_AIR)
{ 
	srand((uint)time(NULL));
	m_perlinSeed = RandInt(0, 100);
	m_locSeed = Randf(-500.f, 500.f);
	
	LakeGeneratorData* envGenData = static_cast<LakeGeneratorData*>(genData);

	String type = envGenData->GetTileType();
	StringUtils::ToLowerCase(type);
	if (type == "lava") {
		m_tileType = TILE_LAVA;
	}
	else if (type == "water") {
		m_tileType = TILE_WATER;
	}
}

STATIC Generator* LakeGenerator::CreateGenerator(EnvironmentGeneratorData* genData) {
	return new LakeGenerator(genData);
}

//---------------------------------------------------------------------------------------------------------------------------
//GENERATION
//---------------------------------------------------------------------------------------------------------------------------
Map* LakeGenerator::GenerateMap(const IntVector2& size) {

	Map* map = new Map(size);
	return map;
}
bool LakeGenerator::GenerateStep(Map* map, int& currentStepNumber) {
	UNREFERENCED_PARAMETER(currentStepNumber);

	IntVector2 mapSize = map->GetSize();

	for (int x = 0; x < mapSize.x - 1; x++) {
		for (int y = 0; y < mapSize.y - 1; y++) {

			Vector2 loc = Vector2(m_locSeed + (float)x, m_locSeed + (float)y);
			float groundLevel = Compute2dPerlinNoise(loc.x, loc.y, 30.f, 2, 0.9f, (float)m_perlinSeed);

			eTileType currTileType = map->GetTileAtLocation(IntVector2(x, y))->GetCurrentTileType();
			if (groundLevel < m_currWaterLevel && currTileType != TILE_STONE) {
				map->GetTileAtLocation(IntVector2(x, y))->SetCurrentTileType(m_tileType);
			}
		}
	}

	m_currWaterLevel += 0.01f;

	return true;
}

void LakeGenerator::GenerateAll(Map* map, int numSteps) {
	int currStepNumber = 0;

	for (int i = 0; i < numSteps; i++) {
		GenerateStep(map, currStepNumber);
	}
}