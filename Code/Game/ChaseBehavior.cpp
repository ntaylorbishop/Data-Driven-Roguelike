#include "Game/ChaseBehavior.hpp"
#include "Game/NPC.hpp"
#include "Engine/General/Core/GameEngine.hpp"
#include "Game/PathFinder.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//CHASE BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
STATIC BehaviorRegistration ChaseBehavior::s_chaseBehaviorRegistration("Chase", &ChaseBehavior::CreateBehavior);

ChaseBehavior::ChaseBehavior(BehaviorData* behaviorData)
	: Behavior(behaviorData)
{ }

STATIC Behavior* ChaseBehavior::CreateBehavior(BehaviorData* behaviorData) {
	return new ChaseBehavior(behaviorData);
}

//---------------------------------------------------------------------------------------------------------------------------
//RUN
//---------------------------------------------------------------------------------------------------------------------------
float ChaseBehavior::CalcUtility(NPC* owningNPC) {

	std::vector<Agent*> nearbyAgents;

	for (std::set<Entity*>::iterator it = owningNPC->m_entitiesInView.begin(); it != owningNPC->m_entitiesInView.end(); it++) {
		if ((*it)->IsAgent()) {
			nearbyAgents.push_back(static_cast<Agent*>(*it));
		}
	}

	std::vector<Agent*> adjacentDislikedEnemies;
	std::vector<Agent*> adjacentHatedEnemies;

	for (unsigned int i = 0; i < nearbyAgents.size(); i++) {
		Agent*& currAgent = nearbyAgents[i];

		eFactionRelationship rel = owningNPC->GetFaction()->GetAgentRelationship(currAgent);

		if (rel == FACTION_RELATIONSHIP_NUETRAL || rel == FACTION_RELATIONSHIP_LIKES || rel == FACTION_RELATIONSHIP_LOVES) {
			continue;
		}

		PathFinder* pathFindToTarget = new PathFinder(owningNPC->GetPosition(), currAgent->GetPosition());
		std::vector<IntVector2> path = pathFindToTarget->FindBestPathFromStartToGoal(owningNPC->GetPosition(), currAgent->GetPosition(), owningNPC->GetMovementProperties());

		if (path.size() > 0) {
			if (owningNPC->GetFaction()->GetAgentRelationship(currAgent) == FACTION_RELATIONSHIP_HATES) {
				adjacentHatedEnemies.push_back(currAgent);
				break;
			}
			else if (owningNPC->GetFaction()->GetAgentRelationship(currAgent) == FACTION_RELATIONSHIP_DISLIKES) {
				adjacentDislikedEnemies.push_back(currAgent);
			}
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
void ChaseBehavior::Run(NPC* owningNPC) {

	if (owningNPC->m_currTargetedEnemy) {
		PathFinder* pathFindToTarget = new PathFinder(owningNPC->GetPosition(), owningNPC->m_currTargetedEnemy->GetPosition());

		std::vector<IntVector2> path = pathFindToTarget->FindBestPathFromStartToGoal(owningNPC->GetPosition(), owningNPC->m_currTargetedEnemy->GetPosition(), owningNPC->GetMovementProperties());
		
		if (path.size() > 0) {
			owningNPC->MoveToLocation(path[path.size() - 2]);
		}
	}
	else {
		g_theApparatusEngine->PrintOutputToConsole("WARNING: No currently selected enemy to chase. This shouldn't happen.", CONSOLE_WARNING);
	}

}
VIRTUAL XMLNode ChaseBehavior::SaveToXML() {
	XMLNode bNode;
	bNode = bNode.createXMLTopNode("Chase");
	return bNode;
}


//---------------------------------------------------------------------------------------------------------------------------
//CHASE BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------

STATIC BehaviorDataRegistration ChaseBehaviorData::s_ChaseBehaviorDataRegistration("Chase", &ChaseBehaviorData::CreateBehaviorData);

ChaseBehaviorData::ChaseBehaviorData(String name)
	: BehaviorData(name)
{
	//Add data node stuff here
}

STATIC BehaviorData* ChaseBehaviorData::CreateBehaviorData(const XMLNode& behaviorDataNode) {
	UNREFERENCED_PARAMETER(behaviorDataNode);

	//Add data node stuff here
	return new ChaseBehaviorData("Chase");
}