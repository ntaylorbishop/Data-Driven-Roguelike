#pragma once

#include <stdlib.h>
#include <time.h>

#include "Engine/Math/IntVector2.hpp"
#include "Game/Map.hpp"
#include "Game/Generator.hpp" 

class CellularAutomatonGenerator : public Generator {
public:
	CellularAutomatonGenerator(EnvironmentGeneratorData* genData);

	static Generator* CreateGenerator(EnvironmentGeneratorData* genData);

	Map* GenerateMap(const IntVector2& size);
	bool GenerateStep(Map* map, int& currentStepNumber);

	void GenerateAll(Map* map, int numSteps);

	static GeneratorRegistration s_CAGRegistration;
private:

};