#include "Game/PickUpBehavior.hpp"
#include "Game/NPC.hpp"
#include "Engine/General/Core/GameEngine.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
STATIC BehaviorRegistration PickUpBehavior::s_meleeAttackBehaviorRegistration("PickUp", &PickUpBehavior::CreateBehavior);

PickUpBehavior::PickUpBehavior(BehaviorData* behaviorData)
	: Behavior(behaviorData)
{
	PickUpBehaviorData* bData;

	try {
		bData = static_cast<PickUpBehaviorData*>(behaviorData);
	}
	catch (int e) {
		UNREFERENCED_PARAMETER(e);
		ERROR_AND_DIE("ERROR: Bad data passed into Melee Attack behavior");
	}

}

STATIC Behavior* PickUpBehavior::CreateBehavior(BehaviorData* behaviorData) {
	return new PickUpBehavior(behaviorData);
}

//---------------------------------------------------------------------------------------------------------------------------
//RUN
//---------------------------------------------------------------------------------------------------------------------------
float PickUpBehavior::CalcUtility(NPC* owningNPC) {
	UNREFERENCED_PARAMETER(owningNPC);

/*
	if (owningNPC->m_itemsInView.size() > 0) {

	}

	std::vector<Agent*> adjacentAgents;
	owningNPC->GetAdjacentAgentsWithinManhattanDistance(1, adjacentAgents);

	std::vector<Agent*> adjacentDislikedEnemies;
	std::vector<Agent*> adjacentHatedEnemies;

	for (unsigned int i = 0; i < adjacentAgents.size(); i++) {
		Agent*& currAgent = adjacentAgents[i];

		if (owningNPC->GetFaction()->GetAgentRelationship(currAgent) == FACTION_RELATIONSHIP_HATES) {
			adjacentHatedEnemies.push_back(currAgent);
		}
		else if (owningNPC->GetFaction()->GetAgentRelationship(currAgent) == FACTION_RELATIONSHIP_DISLIKES) {
			adjacentDislikedEnemies.push_back(currAgent);
		}
	}

	if (adjacentHatedEnemies.size() > 0) {
		owningNPC->m_currTargetedEnemy = adjacentHatedEnemies[0];
		return 1.f;
	}

	if (adjacentDislikedEnemies.size() > 0) {
		owningNPC->m_currTargetedEnemy = adjacentDislikedEnemies[0];
		return 0.9f;
	}
*/

	return 0.f;
}
void PickUpBehavior::Run(NPC* owningNPC) {
	UNREFERENCED_PARAMETER(owningNPC);


/*
	if (owningNPC->m_currTargetedEnemy) {
		owningNPC->SetDamage(m_damage);

		AttackData atkData = g_theCombatSystem->Attack(owningNPC, owningNPC->m_currTargetedEnemy);
	}
	else {
		g_theApparatusEngine->PrintOutputToConsole("WARNING: No curr targeted enemy from NPC " + owningNPC->m_currTargetedEnemy->GetName() + ". Shouldn't happen.", CONSOLE_WARNING);
	}
*/

}
VIRTUAL XMLNode PickUpBehavior::SaveToXML() {
	XMLNode bNode;
	bNode = bNode.createXMLTopNode("PickUp");
	return bNode;
}

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------

STATIC BehaviorDataRegistration PickUpBehaviorData::s_meleeAttackBehaviorDataRegistration("PickUp", &PickUpBehaviorData::CreateBehaviorData);

PickUpBehaviorData::PickUpBehaviorData(String name)
	: BehaviorData(name)
{ }

STATIC BehaviorData* PickUpBehaviorData::CreateBehaviorData(const XMLNode& behaviorDataNode) {
	UNREFERENCED_PARAMETER(behaviorDataNode);

	PickUpBehaviorData* bData = new PickUpBehaviorData("PickUp");

	//Add data node stuff here
	return bData;
}