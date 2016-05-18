#pragma once

#include "Game/Behavior.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//Flee BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
class FleeBehavior : public Behavior {
public:
	FleeBehavior(BehaviorData* genData);

	static Behavior* CreateBehavior(BehaviorData* behaviorData);

	VIRTUAL float CalcUtility(NPC* owningNPC);
	VIRTUAL void Run(NPC* owningNPC);
	VIRTUAL XMLNode SaveToXML();


	static BehaviorRegistration s_FleeBehaviorRegistration;
private:

};

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------
class FleeBehaviorData : public BehaviorData {
public:
	FleeBehaviorData(String name);

	static BehaviorData* CreateBehaviorData(const XMLNode& behaviorDataNode);

	static BehaviorDataRegistration s_FleeBehaviorDataRegistration;

private:
	//INHERITED
	//String m_behaviorName;
};