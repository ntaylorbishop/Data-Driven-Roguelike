#pragma once

#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/EnvironmentGeneratorData.hpp"


class LakeGeneratorData : public EnvironmentGeneratorData {
public:
	LakeGeneratorData(const String& name, int steps, const String& type);

	String GetTileType() const { return m_tileType; }

	static EnvironmentGeneratorData* CreateGeneratorData(const XMLNode& envGenDataNode);

	static EnvironmentGeneratorDataRegistration s_LakeGenDataRegistration;

private:
	//INHERITED
	//String m_generatorName;
	//int m_steps;

	String m_tileType;
};