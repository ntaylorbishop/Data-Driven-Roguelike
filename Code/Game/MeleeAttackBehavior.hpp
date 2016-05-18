#pragma once

#include "Game/GameCommon.hpp"
#include "Game/Behavior.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//MELEE ATTACK BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
class MeleeAttackBehavior : public Behavior {
public:
	MeleeAttackBehavior(BehaviorData* genData);

	static Behavior* CreateBehavior(BehaviorData* behaviorData);

	VIRTUAL float CalcUtility(NPC* owningNPC);
	VIRTUAL void Run(NPC* owningNPC);
	VIRTUAL XMLNode SaveToXML();

	static BehaviorRegistration s_meleeAttackBehaviorRegistration;

private:
	RangeInt m_damage;

};

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------
class MeleeAttackBehaviorData : public BehaviorData {
public:
	MeleeAttackBehaviorData(String name);

	//GETTERS SETTES
	RangeInt GetDamage() const { return m_damage; }
	void SetDamage(const RangeInt& dmg) { m_damage = dmg; }

	//STATIC
	static BehaviorData* CreateBehaviorData(const XMLNode& behaviorDataNode);

	static BehaviorDataRegistration s_meleeAttackBehaviorDataRegistration;

private:
	//INHERITED
	//String m_behaviorName;

	RangeInt m_damage;
};