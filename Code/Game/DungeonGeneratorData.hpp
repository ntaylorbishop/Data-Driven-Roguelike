#pragma once

#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/EnvironmentGeneratorData.hpp"


class DungeonGeneratorData : public EnvironmentGeneratorData {
public:
	DungeonGeneratorData(const String& name, int steps);

	static EnvironmentGeneratorData* CreateGeneratorData(const XMLNode& envGenDataNode);

	static EnvironmentGeneratorDataRegistration s_dungeonGenDataRegistration;

private:
	//INHERITED
	//String m_generatorName;
	//int m_steps;
};