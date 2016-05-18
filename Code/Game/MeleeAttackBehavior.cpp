#include "Game/MeleeAttackBehavior.hpp"
#include "Game/NPC.hpp"
#include "Engine/General/Core/GameEngine.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
STATIC BehaviorRegistration MeleeAttackBehavior::s_meleeAttackBehaviorRegistration("MeleeAttack", &MeleeAttackBehavior::CreateBehavior);

MeleeAttackBehavior::MeleeAttackBehavior(BehaviorData* behaviorData)
	: Behavior(behaviorData)
{ 
	MeleeAttackBehaviorData* bData;
	
	try {
		bData = static_cast<MeleeAttackBehaviorData*>(behaviorData);

		m_damage = bData->GetDamage();
	}
	catch (int e) {
		UNREFERENCED_PARAMETER(e);
		ERROR_AND_DIE("ERROR: Bad data passed into Melee Attack behavior");
	}

}

STATIC Behavior* MeleeAttackBehavior::CreateBehavior(BehaviorData* behaviorData) {
	return new MeleeAttackBehavior(behaviorData);
}

//---------------------------------------------------------------------------------------------------------------------------
//RUN
//---------------------------------------------------------------------------------------------------------------------------
float MeleeAttackBehavior::CalcUtility(NPC* owningNPC) {
	UNREFERENCED_PARAMETER(owningNPC);
	
	std::vector<Agent*> adjacentAgents;
	owningNPC->GetAdjacentAgentsWithinManhattanDistance(1, adjacentAgents);
	owningNPC->SetPrevDamage(m_damage);

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

	return 0.f;
}
void MeleeAttackBehavior::Run(NPC* owningNPC) {

	if (owningNPC->m_currTargetedEnemy) {
		
		AttackData atkData = g_theCombatSystem->Attack(owningNPC, owningNPC->m_currTargetedEnemy);
	}
	else {
		g_theApparatusEngine->PrintOutputToConsole("WARNING: No curr targeted enemy from NPC " + owningNPC->m_currTargetedEnemy->GetName() + ". Shouldn't happen.", CONSOLE_WARNING);
	}
}
VIRTUAL XMLNode MeleeAttackBehavior::SaveToXML() {
	XMLNode bNode;
	bNode = bNode.createXMLTopNode("MeleeAttack");

	String dmg;
	dmg = std::to_string(m_damage.GetMin()) + "~" + std::to_string(m_damage.GetMax());

	bNode.addAttribute("damage", dmg.c_str());

	return bNode;
}


//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------

STATIC BehaviorDataRegistration MeleeAttackBehaviorData::s_meleeAttackBehaviorDataRegistration("MeleeAttack", &MeleeAttackBehaviorData::CreateBehaviorData);

MeleeAttackBehaviorData::MeleeAttackBehaviorData(String name)
	: BehaviorData(name)
{ }

STATIC BehaviorData* MeleeAttackBehaviorData::CreateBehaviorData(const XMLNode& behaviorDataNode) {
	UNREFERENCED_PARAMETER(behaviorDataNode);

	std::vector<XMLUtils::Attribute> bDataAttribs;
	XMLUtils::GetAllAttributesOfNode(behaviorDataNode, bDataAttribs);

	MeleeAttackBehaviorData* bData = new MeleeAttackBehaviorData("MeleeAttack");

	for (unsigned int i = 0; i < bDataAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = bDataAttribs[i];
		StringUtils::ToLowerCase(currAttrib.name);

		bData->SetDamage(NPC_DEFAULT_DAMAGE);

		if (currAttrib.name == "damage") {
			bData->SetDamage(XMLUtils::ParseRangeIntFromXML(currAttrib.value));
		}
	}

	//Add data node stuff here
	return bData;
}