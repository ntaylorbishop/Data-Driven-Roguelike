#pragma once

#include "Game/Behavior.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
class WanderBehavior : public Behavior {
public:
	WanderBehavior(BehaviorData* genData);

	static Behavior* CreateBehavior(BehaviorData* behaviorData);

	VIRTUAL float CalcUtility(NPC* owningNPC);
	VIRTUAL void Run(NPC* owningNPC);
	VIRTUAL XMLNode SaveToXML();

	static BehaviorRegistration s_wanderBehaviorRegistration;
private:

};

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------
class WanderBehaviorData : public BehaviorData {
public:
	WanderBehaviorData(String name);

	static BehaviorData* CreateBehaviorData(const XMLNode& behaviorDataNode);

	static BehaviorDataRegistration s_WanderBehaviorDataRegistration;

private:
	//INHERITED
	//String m_behaviorName;
};