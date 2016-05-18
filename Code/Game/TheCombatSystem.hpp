#pragma once

#include "Engine/Math/MathUtils.hpp"
#include "Game/Entity.hpp"
#include "Game/Agent.hpp"
#include "Game/TheCombatSystem.hpp"
#include "Game/TheAnimationManager.hpp"

class TheCombatSystem;
extern TheCombatSystem* g_theCombatSystem;

//---------------------------------------------------------------------------------------------------------------------------
//CONSTANTS AND GLOBALS
//---------------------------------------------------------------------------------------------------------------------------
extern float SIMULATION_CLOCK; // = 0.0f;
const float SIMULATION_DELTA = 0.1f;

//---------------------------------------------------------------------------------------------------------------------------
//ATTACK DATA
//---------------------------------------------------------------------------------------------------------------------------
struct AttackData {
	Agent* m_target;
	Agent* m_instigator;
	int m_damageDealt;
	float m_chanceToHit;
	bool m_didAttackHit;
};

class TheCombatSystem {
public:
	//STRUCTORS
	TheCombatSystem(std::vector<Entity*> entitiesOnCurrMap);

	void LoadFromSavedGame();

	//Update
	void UpdateTurnOrder(float deltaSeconds);

	//COMBAT
	AttackData Attack(Agent* instigator, Agent* target);
	void DisplayAttackMessage(const AttackData& atkData);

	//GETTERS SETTERS
	uint GetTurnCount() { return m_turnCount; }
	void IncrementTurnCount();

	//SAVE LOADING
	XMLNode SaveToXML() const;
	static void LoadFromXML(const XMLNode& node);

	uint m_turnPlayerDied;
	uint m_numMonstersKilled;

	static uint s_loadedTurnCount;
	static uint s_loadedNumMonstersKilled;

private:
	std::multimap<float, Agent*> m_agents;
	uint m_turnCount;

};
typedef std::multimap<float, Agent*> TurnOrderMap;
typedef std::multimap<float, Agent*>::iterator TurnOrderMapIter;
typedef std::pair<float, Agent*> TurnOrderMapPair;