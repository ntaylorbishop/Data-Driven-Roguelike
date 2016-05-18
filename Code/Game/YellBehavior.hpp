#pragma once

#include "Game/Behavior.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//Yell BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
class YellBehavior : public Behavior {
public:
	YellBehavior(BehaviorData* genData);

	static Behavior* CreateBehavior(BehaviorData* behaviorData);

	VIRTUAL float CalcUtility(NPC* owningNPC);
	VIRTUAL void Run(NPC* owningNPC);
	VIRTUAL XMLNode SaveToXML();


	static BehaviorRegistration s_YellBehaviorRegistration;
private:

};

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------
class YellBehaviorData : public BehaviorData {
public:
	YellBehaviorData(String name);

	static BehaviorData* CreateBehaviorData(const XMLNode& behaviorDataNode);

	static BehaviorDataRegistration s_YellBehaviorDataRegistration;

private:
	//INHERITED
	//String m_behaviorName;
};