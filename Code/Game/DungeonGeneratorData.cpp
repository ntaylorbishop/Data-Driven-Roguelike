#include "Game/DungeonGeneratorData.hpp"

STATIC EnvironmentGeneratorDataRegistration DungeonGeneratorData::s_dungeonGenDataRegistration("Dungeon", &DungeonGeneratorData::CreateGeneratorData);

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
DungeonGeneratorData::DungeonGeneratorData(const String& name, int steps)
	: EnvironmentGeneratorData(name, steps)
{ }

STATIC EnvironmentGeneratorData* DungeonGeneratorData::CreateGeneratorData(const XMLNode& envGenDataNode) {

	String generatorName = envGenDataNode.getAttribute(0).lpszValue;
	String stepsAttrib = envGenDataNode.getAttribute(1).lpszValue;
	int steps = XMLUtils::ParseIntFromXML(stepsAttrib);

	return new DungeonGeneratorData(generatorName, steps);
}