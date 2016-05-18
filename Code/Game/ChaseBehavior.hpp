#pragma once

#include "Game/Behavior.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//CHASE BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
class ChaseBehavior : public Behavior {
public:
	ChaseBehavior(BehaviorData* genData);

	static Behavior* CreateBehavior(BehaviorData* behaviorData);

	VIRTUAL float CalcUtility(NPC* owningNPC);
	VIRTUAL void Run(NPC* owningNPC);
	VIRTUAL XMLNode SaveToXML();

	static BehaviorRegistration s_chaseBehaviorRegistration;
private:

};

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------
class ChaseBehaviorData : public BehaviorData {
public:
	ChaseBehaviorData(String name);

	static BehaviorData* CreateBehaviorData(const XMLNode& behaviorDataNode);

	static BehaviorDataRegistration s_ChaseBehaviorDataRegistration;

private:
	//INHERITED
	//String m_behaviorName;
};