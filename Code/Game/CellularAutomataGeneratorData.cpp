#include "Game/CellularAutomataGeneratorData.hpp"

STATIC EnvironmentGeneratorDataRegistration CellularAutomataGeneratorData::s_CAGGenDataRegistration("CellularAutomata", &CellularAutomataGeneratorData::CreateGeneratorData);

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
CellularAutomataGeneratorData::CellularAutomataGeneratorData(const String& name, int steps) 
	: EnvironmentGeneratorData(name, steps)
{ }

STATIC EnvironmentGeneratorData* CellularAutomataGeneratorData::CreateGeneratorData(const XMLNode& envGenDataNode) {

	String generatorName = envGenDataNode.getAttribute(0).lpszValue;
	String stepsAttrib = envGenDataNode.getAttribute(1).lpszValue;
	int steps = XMLUtils::ParseIntFromXML(stepsAttrib);

	return new CellularAutomataGeneratorData(generatorName, steps);
}