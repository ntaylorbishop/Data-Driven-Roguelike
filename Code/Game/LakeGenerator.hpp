#pragma once

#include <stdlib.h>
#include <time.h>

#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/PerlinNoise.hpp"
#include "Game/Map.hpp"
#include "Game/Generator.hpp" 
#include "Game/LakeGeneratorData.hpp"


class LakeGenerator : public Generator {
public:
	LakeGenerator(EnvironmentGeneratorData* genData);

	static Generator* CreateGenerator(EnvironmentGeneratorData* genData);

	Map* GenerateMap(const IntVector2& size);
	bool GenerateStep(Map* map, int& currentStepNumber);

	void GenerateAll(Map* map, int numSteps);

	static GeneratorRegistration s_LakeRegistration;

private:
	float m_currWaterLevel;
	uint m_perlinSeed;
	float m_locSeed;

	eTileType m_tileType;
};