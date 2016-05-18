#include "Game/YellBehavior.hpp"
#include "Game/NPC.hpp"
#include "Engine/General/Core/GameEngine.hpp"
#include "Game/PathFinder.hpp"
#include "Game/TheMessageLog.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//Yell BEHAVIOR
//---------------------------------------------------------------------------------------------------------------------------
STATIC BehaviorRegistration YellBehavior::s_YellBehaviorRegistration("Yell", &YellBehavior::CreateBehavior);

YellBehavior::YellBehavior(BehaviorData* behaviorData)
	: Behavior(behaviorData)
{ }

STATIC Behavior* YellBehavior::CreateBehavior(BehaviorData* behaviorData) {
	return new YellBehavior(behaviorData);
}

//---------------------------------------------------------------------------------------------------------------------------
//RUN
//---------------------------------------------------------------------------------------------------------------------------
float YellBehavior::CalcUtility(NPC* owningNPC) {

	std::vector<Agent*> nearbyAgents;
	owningNPC->GetAdjacentAgentsWithinManhattanDistance(10, nearbyAgents);

	bool canSeePlayer = false;
	for (unsigned int i = 0; i < nearbyAgents.size(); i++) {
		if (nearbyAgents[i]->IsPlayer()) {
			canSeePlayer = true;
		}
	}

	return (canSeePlayer) ? (1.f) : (0.f);
}

void YellBehavior::Run(NPC* owningNPC) {

	std::vector<String> possibleStrs = {
		StringUtils::Stringf("%s yells: \"Is that human flesh I smell...\"", owningNPC->GetName().c_str()),
		StringUtils::Stringf("%s yells: \"HUMAN! GET HIM!\"", owningNPC->GetName().c_str()),
		StringUtils::Stringf("%s yells: \"Oi! Human! I have something for you!\"", owningNPC->GetName().c_str()),
		StringUtils::Stringf("%s yells: \"Come feel the steel of my blade human!\"", owningNPC->GetName().c_str())
	};

	int which = RandIntZeroToSize(possibleStrs.size());

	g_theMessageLog->LogMessage(possibleStrs[which], PRIORITY_HIGH);
}
VIRTUAL XMLNode YellBehavior::SaveToXML() {
	XMLNode bNode;
	bNode = bNode.createXMLTopNode("Yell");
	return bNode;
}


//---------------------------------------------------------------------------------------------------------------------------
//YELL BEHAVIOR DATA
//---------------------------------------------------------------------------------------------------------------------------

STATIC BehaviorDataRegistration YellBehaviorData::s_YellBehaviorDataRegistration("Yell", &YellBehaviorData::CreateBehaviorData);

YellBehaviorData::YellBehaviorData(String name)
	: BehaviorData(name)
{
	//Add data node stuff here
}

STATIC BehaviorData* YellBehaviorData::CreateBehaviorData(const XMLNode& behaviorDataNode) {
	UNREFERENCED_PARAMETER(behaviorDataNode);

	//Add data node stuff here
	return new YellBehaviorData("Yell");
}