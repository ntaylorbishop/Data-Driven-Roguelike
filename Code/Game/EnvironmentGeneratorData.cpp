#include "Game/EnvironmentGeneratorData.hpp"

STATIC std::map<String, EnvironmentGeneratorDataRegistration*>* EnvironmentGeneratorDataRegistration::s_envGenDataRegistrationMap = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//ENVIRONMENT GENERATION PROCESS DATA
//---------------------------------------------------------------------------------------------------------------------------
EnvironmentGeneratorData::EnvironmentGeneratorData() 
	: m_generatorName("NULL")
	, m_steps(-1)
{ }
EnvironmentGeneratorData::EnvironmentGeneratorData(const String& generatorName, int steps) 
	: m_generatorName(generatorName)
	, m_steps(steps)
{ }

String EnvironmentGeneratorData::GetGeneratorName() const {
	return m_generatorName;
}
int EnvironmentGeneratorData::GetGeneratorNumSteps() const {
	return m_steps;
}

//---------------------------------------------------------------------------------------------------------------------------
//GENERATOR REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
EnvironmentGeneratorDataRegistration::EnvironmentGeneratorDataRegistration(const String& name, EGPDCreationFunc* creationFunc) 
	: m_name(name)
	, m_creationFunc(creationFunc)
{
	if (!s_envGenDataRegistrationMap) {
		s_envGenDataRegistrationMap = new std::map<String, EnvironmentGeneratorDataRegistration*>;
	}

	std::map<String, EnvironmentGeneratorDataRegistration*>::iterator findGeneratorIt = s_envGenDataRegistrationMap->find(name);

	if (findGeneratorIt == s_envGenDataRegistrationMap->end()) {
		s_envGenDataRegistrationMap->insert(std::pair<String, EnvironmentGeneratorDataRegistration*>(name, this));
	}
	else {
		String str = "ERROR: " + name + "ALREADY EXISTS IN ENV GEN DATA REGISTRATION";
		DebuggerPrintf(str.c_str());
	}
}
STATIC EnvironmentGeneratorData* EnvironmentGeneratorDataRegistration::CreateEGPDByName(const String& name, const XMLNode& genNode) {
	ASSERT_OR_DIE(s_envGenDataRegistrationMap != nullptr, "ENV GEN DATA REGISTRATION MAP IS NULL");

	std::map<String, EnvironmentGeneratorDataRegistration*>::iterator findNameIt = s_envGenDataRegistrationMap->find(name);
	ASSERT_OR_DIE(findNameIt != s_envGenDataRegistrationMap->end(), "ERROR: GENERATOR DATA DOES NOT EXIST");

	if (findNameIt == s_envGenDataRegistrationMap->end()) {
		return nullptr;
	}

	EnvironmentGeneratorDataRegistration* genDataRegistration = findNameIt->second;
	EnvironmentGeneratorData* genData = (*genDataRegistration->m_creationFunc)(genNode);

	return genData;
}