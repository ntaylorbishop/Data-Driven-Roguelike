#include "Game/FleeBehavior.hpp"
#include "Game/NPC.hpp"
#include "Engine/General/Core/GameEngine.hpp"
#include "Game/PathFinder.hpp"
#include "Game/TheMessageLog.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//Flee BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
STATIC BehaviorRegistration FleeBehavior::s_FleeBehaviorRegistration("Flee", &FleeBehavior::CreateBehavior);

FleeBehavior::FleeBehavior(BehaviorData* behaviorData)
	: Behavior(behaviorData)
{ }

STATIC Behavior* FleeBehavior::CreateBehavior(BehaviorData* behaviorData) {
	return new FleeBehavior(behaviorData);
}

//---------------------------------------------------------------------------------------------------------------------------
//RUN
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL float FleeBehavior::CalcUtility(NPC* owningNPC) {

	if (((float)owningNPC->GetHealth() / (float)owningNPC->GetMaxHealth()) >= 0.4f) {
		return 0.f;
	}
	else {
		return 0.8f;
	}
}
VIRTUAL void FleeBehavior::Run(NPC* owningNPC) {

	std::vector<Agent*> adjacentAgents;
	owningNPC->GetAdjacentAgentsWithinManhattanDistance(1, adjacentAgents);

	std::vector<IntVector2> potentialSpotsToFleeTo = {
		owningNPC->GetPosition() + NORTH,
		owningNPC->GetPosition() + NORTHEAST,
		owningNPC->GetPosition() + EAST,
		owningNPC->GetPosition() + SOUTHEAST,
		owningNPC->GetPosition() + SOUTH,
		owningNPC->GetPosition() + SOUTHWEST,
		owningNPC->GetPosition() + WEST,
		owningNPC->GetPosition() + NORTHWEST
	};

	std::vector<IntVector2>::iterator locIter;
	for (locIter = potentialSpotsToFleeTo.begin(); locIter != potentialSpotsToFleeTo.end();) {
		Tile* currTile = owningNPC->GetBelongingMap()->GetTileAtLocation(*locIter);

		if (currTile && currTile->IsValidForAgent(owningNPC->GetMovementProperties())) {
			locIter++;
		}
		else {
			locIter = potentialSpotsToFleeTo.erase(locIter);
		}
	}

	int randLoc = RandIntZeroToSize(potentialSpotsToFleeTo.size());
	owningNPC->MoveToLocation(potentialSpotsToFleeTo[randLoc]);

	String disp = StringUtils::Stringf("%s begins to flee!", owningNPC->GetName().c_str());
	if (g_theWorld->m_thePlayer->CanSeeEntity(owningNPC)) {
		g_theMessageLog->LogMessage(disp, PRIORITY_HIGH);
	}
}
VIRTUAL XMLNode FleeBehavior::SaveToXML() {
	XMLNode bNode;
	bNode = bNode.createXMLTopNode("Flee");
	return bNode;
}

//---------------------------------------------------------------------------------------------------------------------------
//Flee BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------

STATIC BehaviorDataRegistration FleeBehaviorData::s_FleeBehaviorDataRegistration("Flee", &FleeBehaviorData::CreateBehaviorData);

FleeBehaviorData::FleeBehaviorData(String name)
	: BehaviorData(name)
{
	//Add data node stuff here
}

STATIC BehaviorData* FleeBehaviorData::CreateBehaviorData(const XMLNode& behaviorDataNode) {
	UNREFERENCED_PARAMETER(behaviorDataNode);

	//Add data node stuff here
	return new FleeBehaviorData("Flee");
}