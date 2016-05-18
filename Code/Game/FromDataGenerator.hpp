#pragma once

#include <stdlib.h>
#include <time.h>

#include "Engine/Math/IntVector2.hpp"
#include "Engine/General/Utils/ErrorWarningAssert.hpp"
#include "ThirdParty/Parsers/xmlParser.h"
#include "Game/Map.hpp"
#include "Game/Generator.hpp" 
#include "Game/FromDataGeneratorData.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTS
//---------------------------------------------------------------------------------------------------------------------------
struct LegendNode {
	LegendNode(char rep, eTileType type) : m_representation(rep), m_tileType(type) {}
	char m_representation;
	eTileType m_tileType;
};

//---------------------------------------------------------------------------------------------------------------------------
//FROM DATA GENERATOR
//---------------------------------------------------------------------------------------------------------------------------
class FromDataGenerator : public Generator {
public:
	//STRUCTORS
	FromDataGenerator(EnvironmentGeneratorData* genData);
	void ParseData(const XMLNode& root);

	//GENERATION
	Map* GenerateMap(const IntVector2& size);
	bool GenerateStep(Map* map, int& currentStepNumber);
	void GenerateAll(Map* map, int numSteps);


	//STATIC
	static Generator* CreateGenerator(EnvironmentGeneratorData* genData);
	static GeneratorRegistration s_fromDataRegistration;

private:
	//PRIVATE MEMBERS FUNCTIONS
	Map* ParseDataIntoTiles();
	eTileType GetTileTypeFromLegend(char currTileChar);

private:
	//INHERITED
	//EnvironmentGeneratorData* m_generatorData;

	std::vector<LegendNode> m_legendNodes;
	String m_mapData;
};