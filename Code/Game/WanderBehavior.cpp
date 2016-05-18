#include "Game/WanderBehavior.hpp"
#include "Game/NPC.hpp"
#include "Game/Map.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
STATIC BehaviorRegistration WanderBehavior::s_wanderBehaviorRegistration("Wander", &WanderBehavior::CreateBehavior);

WanderBehavior::WanderBehavior(BehaviorData* behaviorData)
	: Behavior(behaviorData)
{ 
}

STATIC Behavior* WanderBehavior::CreateBehavior(BehaviorData* behaviorData) {
	return new WanderBehavior(behaviorData);
}

//---------------------------------------------------------------------------------------------------------------------------
//RUN
//---------------------------------------------------------------------------------------------------------------------------
float WanderBehavior::CalcUtility(NPC* owningNPC) {
	UNREFERENCED_PARAMETER(owningNPC);

	return 0.5f;
}
void WanderBehavior::Run(NPC* owningNPC) {
	int chanceToRest = RandInt(1, 2);

	if (chanceToRest == 2) {
		Map* map = owningNPC->GetBelongingMap();
		IntVector2 currLoc = owningNPC->GetPosition();

		std::vector<IntVector2> locsToGoTo = map->GetValidLocsToMoveToAroundLoc(currLoc, owningNPC->GetMovementProperties());
		int whichLoc = RandIntZeroToSize(locsToGoTo.size());
		owningNPC->SetPosition(locsToGoTo[whichLoc]);

	}
}
VIRTUAL XMLNode WanderBehavior::SaveToXML() {
	XMLNode bNode;
	bNode = bNode.createXMLTopNode("Wander");
	return bNode;
}

//---------------------------------------------------------------------------------------------------------------------------
//WANDER BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------

STATIC BehaviorDataRegistration WanderBehaviorData::s_WanderBehaviorDataRegistration("Wander", &WanderBehaviorData::CreateBehaviorData);

WanderBehaviorData::WanderBehaviorData(String name)
	: BehaviorData(name)
{ 
	//Add data node stuff here
}

STATIC BehaviorData* WanderBehaviorData::CreateBehaviorData(const XMLNode& behaviorDataNode) {
	UNREFERENCED_PARAMETER(behaviorDataNode);

	//Add data node stuff here
	return new WanderBehaviorData("Wander");
}