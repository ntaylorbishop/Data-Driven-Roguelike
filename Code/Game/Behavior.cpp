#include "Game/Behavior.hpp"

STATIC std::map<String, BehaviorRegistration*>* BehaviorRegistration::s_behaviorRegistrationMap = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//GENERATOR
//---------------------------------------------------------------------------------------------------------------------------
Behavior::Behavior(BehaviorData* behaviorData)
	: m_behaviorData(behaviorData)
{ 
	m_name = behaviorData->GetBehaviorName();
}

//---------------------------------------------------------------------------------------------------------------------------
//GENERATOR REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
BehaviorRegistration::BehaviorRegistration(const String& name, BehaviorCreationFunc* creationFunc)
	: m_name(name)
	, m_creationFunc(creationFunc)
{
	if (!s_behaviorRegistrationMap) {
		s_behaviorRegistrationMap = new std::map<String, BehaviorRegistration*>;
	}

	std::map<String, BehaviorRegistration*>::iterator findGeneratorIt = s_behaviorRegistrationMap->find(name);

	if (findGeneratorIt == s_behaviorRegistrationMap->end()) {
		s_behaviorRegistrationMap->insert(std::pair<String, BehaviorRegistration*>(name, this));
	}
	else {
		String str = "ERROR: " + name + "ALREADY EXISTS IN BEHAVIOR REGISTRATION";
		DebuggerPrintf(str.c_str());
	}

}

STATIC Behavior* BehaviorRegistration::CreateBehaviorByName(const String& name, BehaviorData* behaviorData) {
	ASSERT_OR_DIE(behaviorData != nullptr, "ERROR: MUST SUPPLY BEHAVIOR DATA WHEN CREATING A BEHAVIOR");
	ASSERT_OR_DIE(s_behaviorRegistrationMap != nullptr, "BEHAVIOR REGISTRATION MAP IS NULL");

	std::map<String, BehaviorRegistration*>::iterator findNameIt = s_behaviorRegistrationMap->find(name);
	ASSERT_OR_DIE(findNameIt != s_behaviorRegistrationMap->end(), "ERROR: BEHAVIOR DOES NOT EXIST");

	BehaviorRegistration* behaviorRegistration = findNameIt->second;
	Behavior* behavior = (*behaviorRegistration->m_creationFunc)(behaviorData);

	return behavior;
}


//---------------------------------------------------------------------------------------------------------------------------
//BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------

STATIC std::map<String, BehaviorDataRegistration*>* BehaviorDataRegistration::s_behaviorDataRegistrationMap = nullptr;

BehaviorData::BehaviorData()
	: m_behaviorName("NULL")
{ }
BehaviorData::BehaviorData(const String& behaviorName)
	: m_behaviorName(behaviorName)
{ }

String BehaviorData::GetBehaviorName() const {
	return m_behaviorName;
}

//---------------------------------------------------------------------------------------------------------------------------
//BEHAVIOR DATA REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
BehaviorDataRegistration::BehaviorDataRegistration(const String& name, BDParseFunc* creationFunc)
	: m_name(name)
	, m_creationFunc(creationFunc)
{
	if (!s_behaviorDataRegistrationMap) {
		s_behaviorDataRegistrationMap = new std::map<String, BehaviorDataRegistration*>;
	}

	std::map<String, BehaviorDataRegistration*>::iterator findGeneratorIt = s_behaviorDataRegistrationMap->find(name);

	if (findGeneratorIt == s_behaviorDataRegistrationMap->end()) {
		s_behaviorDataRegistrationMap->insert(std::pair<String, BehaviorDataRegistration*>(name, this));
	}
	else {
		String str = "ERROR: " + name + "ALREADY EXISTS IN ENV GEN DATA REGISTRATION";
		DebuggerPrintf(str.c_str());
	}
}
STATIC BehaviorData* BehaviorDataRegistration::CreateBDDataByName(const String& name, const XMLNode& genNode) {
	ASSERT_OR_DIE(s_behaviorDataRegistrationMap != nullptr, "ENV GEN DATA REGISTRATION MAP IS NULL");

	std::map<String, BehaviorDataRegistration*>::iterator findNameIt = s_behaviorDataRegistrationMap->find(name);
	ASSERT_OR_DIE(findNameIt != s_behaviorDataRegistrationMap->end(), "ERROR: GENERATOR DATA DOES NOT EXIST");

	if (findNameIt == s_behaviorDataRegistrationMap->end()) {
		return nullptr;
	}

	BehaviorDataRegistration* genDataRegistration = findNameIt->second;
	BehaviorData* genData = (*genDataRegistration->m_creationFunc)(genNode);

	return genData;
}