#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

//---------------------------------------------------------------------------------------------------------------------------
//ENVIRONMENT GENERATION PROCESS DATA
//---------------------------------------------------------------------------------------------------------------------------
class EnvironmentGeneratorData {
public:
	//STRUCTORS
	EnvironmentGeneratorData();
	EnvironmentGeneratorData(const String& generatorName, int steps);

	//GETTERS SETTERS
	String GetGeneratorName() const;
	int GetGeneratorNumSteps() const;

protected:
	String m_generatorName;
	int m_steps;
};

typedef EnvironmentGeneratorData* (EGPDCreationFunc)(const XMLNode& genNode);


//---------------------------------------------------------------------------------------------------------------------------
//GENERATOR REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
class EnvironmentGeneratorDataRegistration {
public:
	EnvironmentGeneratorDataRegistration(const String& name, EGPDCreationFunc* creationFunc);
	static EnvironmentGeneratorData* CreateEGPDByName(const String& name, const XMLNode& genNode);

	static std::map<String, EnvironmentGeneratorDataRegistration*>* EnvironmentGeneratorDataRegistration::s_envGenDataRegistrationMap;

private:
	String m_name;
	EGPDCreationFunc* m_creationFunc;
};