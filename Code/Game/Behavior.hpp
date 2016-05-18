#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

class BehaviorData;
class NPC;

//---------------------------------------------------------------------------------------------------------------------------
//BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
class Behavior {
public:
	Behavior(BehaviorData* behaviorData);

	String GetName() const { return m_name; }
	BehaviorData* GetBehaviorData() { return m_behaviorData; }

	virtual float CalcUtility(NPC* owningNPC) = 0;
	virtual void Run(NPC* owningNPC) = 0;
	virtual XMLNode SaveToXML() = 0;

protected:
	String m_name;
	BehaviorData* m_behaviorData;
};

typedef Behavior* (BehaviorCreationFunc)(BehaviorData* behaviorData);

//---------------------------------------------------------------------------------------------------------------------------
//BEHAVIOR REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
class BehaviorRegistration {
public:
	BehaviorRegistration(const String& name, BehaviorCreationFunc* creationFunc);

	static Behavior* CreateBehaviorByName(const String& name, BehaviorData* behaviorData);
	static std::map<String, BehaviorRegistration*>* BehaviorRegistration::s_behaviorRegistrationMap;

private:
	String m_name;
	BehaviorCreationFunc* m_creationFunc;

};

//---------------------------------------------------------------------------------------------------------------------------
//BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------
class BehaviorData {
public:
	//STRUCTORS
	BehaviorData();
	BehaviorData(const String& behaviorName);

	//GETTERS SETTERS
	String GetBehaviorName() const;

protected:
	String m_behaviorName;
};

typedef BehaviorData* (BDParseFunc)(const XMLNode& genNode);

//---------------------------------------------------------------------------------------------------------------------------
//BEHAVIOR DATA REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
class BehaviorDataRegistration {
public:
	BehaviorDataRegistration(const String& name, BDParseFunc* creationFunc);
	static BehaviorData* CreateBDDataByName(const String& name, const XMLNode& genNode);

	static std::map<String, BehaviorDataRegistration*>* BehaviorDataRegistration::s_behaviorDataRegistrationMap;

private:
	String m_name;
	BDParseFunc* m_creationFunc;
};