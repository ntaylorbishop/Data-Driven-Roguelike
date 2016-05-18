#pragma once

#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/EnvironmentGeneratorData.hpp"


class FromDataGeneratorData : public EnvironmentGeneratorData {
public:
	FromDataGeneratorData(const String& name, const String& fileName);
	String GetFileName() const;

	static EnvironmentGeneratorData* CreateGeneratorData(const XMLNode& envGenDataNode);
	static EnvironmentGeneratorDataRegistration s_fromDataGenDataRegistration;

private:
	//INHERITED
	//String m_generatorName;
	//int m_steps;

	String m_fileName;
};