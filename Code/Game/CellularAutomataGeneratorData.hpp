#pragma once

#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/EnvironmentGeneratorData.hpp"


class CellularAutomataGeneratorData : public EnvironmentGeneratorData {
public:
	CellularAutomataGeneratorData(const String& name, int steps);

	static EnvironmentGeneratorData* CreateGeneratorData(const XMLNode& envGenDataNode);

	static EnvironmentGeneratorDataRegistration s_CAGGenDataRegistration;

private:
	//INHERITED
	//String m_generatorName;
	//int m_steps;
};