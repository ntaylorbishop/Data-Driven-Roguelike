#include "Game/TheCombatSystem.hpp"
#include "Game/TheWorld.hpp"
#include "Game/TheMessageLog.hpp"

float SIMULATION_CLOCK = 0.0f;
TheCombatSystem* g_theCombatSystem = nullptr;

STATIC uint TheCombatSystem::s_loadedTurnCount = 0;
STATIC uint TheCombatSystem::s_loadedNumMonstersKilled = 0;


//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
TheCombatSystem::TheCombatSystem(std::vector<Entity*> entitiesOnCurrMap) 
	: m_turnCount(0)
	, m_numMonstersKilled(0)
{ 
	m_agents.insert(TurnOrderMapPair(1.f, g_theWorld->m_thePlayer));

	for (unsigned int i = 0; i < entitiesOnCurrMap.size(); i++) {
		Agent* currAgent = nullptr;

		if (entitiesOnCurrMap[i]->IsAgent()) {
			currAgent = dynamic_cast<Agent*>(entitiesOnCurrMap[i]);
			m_agents.insert(TurnOrderMapPair(currAgent->GetMoveSpeed(), currAgent));
		}
	}
}

void TheCombatSystem::LoadFromSavedGame() {

	m_numMonstersKilled = s_loadedNumMonstersKilled;
	m_turnCount = s_loadedTurnCount;
}


//---------------------------------------------------------------------------------------------------------------------------
//Update
//---------------------------------------------------------------------------------------------------------------------------
void TheCombatSystem::UpdateTurnOrder(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

	bool isSimulating = true;
	bool advanceTime = true;

	if (!g_theWorld->m_thePlayer) {
		return;
	}

	while(isSimulating) {
		TurnOrderMapIter agentIter = m_agents.begin();
		Agent* currAgent = agentIter->second;

		if (!currAgent)
			break;

		if (agentIter->first > SIMULATION_CLOCK) {
			break;
		}

		//Get rid of this if you want it to be real-time and set sim clock to world seconds delta
		if (currAgent && !currAgent->IsReadyToUpdate()) {
			advanceTime = false;
			break;
		}

		m_agents.erase(agentIter);

		float duration = 0.f;
		if (currAgent->IsAlive()) {
			currAgent->Run();
			duration = currAgent->GetMoveSpeed();
		}

		if (currAgent->IsAlive()) {
			m_agents.insert(TurnOrderMapPair(SIMULATION_CLOCK + duration, currAgent));
		}
	}

	if (advanceTime) {
		if (!g_theWorld->m_playerJustDied) {
			SIMULATION_CLOCK += SIMULATION_DELTA;
		}
		else {
			SIMULATION_CLOCK += deltaSeconds;
		}
	}

	TurnOrderMapIter agentIter = m_agents.begin();

	if (m_agents.size() <= 1)
		return;

	for (agentIter = m_agents.begin(); agentIter != m_agents.end(); ) {
		if (!agentIter->second->IsAlive()) {
			TurnOrderMapIter save = agentIter;
			++save;
			m_agents.erase(agentIter);
			agentIter = save;
		}
		else {
			++agentIter;
		}
	}

	g_theWorld->CleanUpDeadEntities();


}

//---------------------------------------------------------------------------------------------------------------------------
//COMBAT
//---------------------------------------------------------------------------------------------------------------------------
AttackData TheCombatSystem::Attack(Agent* instigator, Agent* target) {
	AttackData atkData;

	atkData.m_target = target;
	atkData.m_instigator = instigator;
	atkData.m_damageDealt = instigator->GetDamage().Roll() - target->GetArmor();
	if (atkData.m_damageDealt < 0) {
		atkData.m_damageDealt = 0;
	}
	atkData.m_chanceToHit = 1.f;
	atkData.m_didAttackHit = true;

	target->DecrementHealth(atkData.m_damageDealt);
	g_theAnimationManager->PlayAnimation(target->GetPosition(), RGBA::RED, target->GetGlyph());


	if (!instigator->IsPlayer()) {
		NPC* instNPC = static_cast<NPC*>(instigator);

		if (!target->IsAlive()) {
			instNPC->m_currTargetedEnemy = nullptr;
		}
	}


	DisplayAttackMessage(atkData);

	if (!target->IsPlayer()) {
		NPC* targetNPC = static_cast<NPC*>(target);
		targetNPC->NotifyAttacked(atkData);
	}
	
	return atkData;
}
void TheCombatSystem::DisplayAttackMessage(const AttackData& atkData) {
	//GET PRIORITY OF MESSAGE DISPLAY
	if (atkData.m_instigator->IsPlayer()) {
		String disp = StringUtils::Stringf("Attacked %s for %i damage!", atkData.m_target->GetName().c_str(), atkData.m_damageDealt);
		g_theMessageLog->LogMessage(disp, PRIORITY_HIGH);
		return;
	}

	if (atkData.m_target->IsPlayer()) {
		String disp = StringUtils::Stringf("%s attacked you for %i damage!", atkData.m_instigator->GetName().c_str(), atkData.m_damageDealt);
		g_theMessageLog->LogMessage(disp, PRIORITY_HIGH);
		return;
	}

	//Player not involved
	String disp = StringUtils::Stringf("%s attacked %s for %i damage!", atkData.m_instigator->GetName().c_str(), atkData.m_target->GetName().c_str(), atkData.m_damageDealt);
	eMsgPriority msgPri = NUM_PRIORITIES;

	if (g_theWorld->m_thePlayer->CanSeeEntity(atkData.m_instigator) || g_theWorld->m_thePlayer->CanSeeEntity(atkData.m_target)) {
		msgPri = PRIORITY_HIGH;
	}
	else {
		msgPri = PRIORITY_LOW;
	}
}
void TheCombatSystem::IncrementTurnCount() {
	m_turnCount++;
}

//SAVE LOADING
XMLNode TheCombatSystem::SaveToXML() const {
	XMLNode turnNode = turnNode.createXMLTopNode("TurnInfo");

	turnNode.addAttribute("NumTurns", std::to_string(m_turnCount).c_str());
	turnNode.addAttribute("MonstersKilled", std::to_string(m_numMonstersKilled).c_str());

	return turnNode;
}
STATIC void TheCombatSystem::LoadFromXML(const XMLNode& node) {

	std::vector<XMLUtils::Attribute> itemAttribs;
	XMLUtils::GetAllAttributesOfNode(node, itemAttribs);

	for (unsigned int i = 0; i < itemAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = itemAttribs[i];
		String currAttribName = currAttrib.name;
		String currAttribVal = currAttrib.value;

		if (currAttribName == "NumTurns") {
			s_loadedTurnCount = XMLUtils::ParseIntFromXML(currAttribVal);
		}
		else if (currAttribName == "MonstersKilled") {
			s_loadedNumMonstersKilled = XMLUtils::ParseIntFromXML(currAttribVal);
		}
	}
}