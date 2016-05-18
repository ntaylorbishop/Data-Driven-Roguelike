#include "Game/NPC.hpp"
#include "Game/Map.hpp"
#include "Game/PathFinder.hpp"
#include "Engine/General/Core/GameEngine.hpp"
#include "Game/TheMessageLog.hpp"
#include "Game/Equipment.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
NPC::NPC(const XMLNode& templateDataNode) 
	: Agent()
	, m_currPathFind(nullptr)
	, m_currTargetedAlly(nullptr)
	, m_currTargetedEnemy(nullptr)
	, m_movementProperties(0)
{ 

	m_inventory = new Inventory(true);
	m_equipment = new Equipment();

	PopulateFromXMLNode(templateDataNode);

	m_id = Entity::s_entityIDs++;
}
NPC::NPC(NPC* templateNPC) 
	: m_currPathFind(nullptr)
	, m_currTargetedAlly(nullptr)
	, m_currTargetedEnemy(nullptr)
	, m_movementProperties(0)
{
	for (unsigned int i = 0; i < templateNPC->m_behaviors.size(); i++) {
		Behavior* newBehavior = BehaviorRegistration::CreateBehaviorByName(templateNPC->m_behaviors[i]->GetName(), templateNPC->m_behaviors[i]->GetBehaviorData());
		m_behaviors.push_back(newBehavior);
	}

	m_health = m_maxHealth;
	m_moveSpeed = templateNPC->GetMoveSpeed();
	m_isAlive = true;
	m_damage = templateNPC->GetDamage();
	m_armor = templateNPC->GetArmor();

	m_position = templateNPC->GetPosition();
	m_glyph = templateNPC->GetGlyph();
	m_color = templateNPC->GetColor();
	m_backgroundColor = templateNPC->GetBackgroundColor();
	m_name = templateNPC->GetName();
	m_belongingMap = templateNPC->GetBelongingMap();

	m_inventory = new Inventory(true);
	m_equipment = new Equipment();
	m_inventory->Clone(templateNPC->GetInventory());
	m_equipment->Clone(templateNPC->GetEquipment());
	m_movementProperties = templateNPC->GetMovementProperties();
	

	if (templateNPC->GetFaction()) {
		m_faction = new Faction(templateNPC->GetFaction());
	}

	m_currPathFind = nullptr;
	m_id = Entity::s_entityIDs++;
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void NPC::Run() {
	if (m_behaviors.size() > 0) {
		Behavior* behaviorToRun = m_behaviors[0];

		for (unsigned int i = 1; i < m_behaviors.size(); i++) {

			if (m_behaviors[i]->CalcUtility(this) > behaviorToRun->CalcUtility(this)) {
				behaviorToRun = m_behaviors[i];
			}
		}

		behaviorToRun->Run(this);
	}
	else {
		g_theApparatusEngine->PrintOutputToConsole("WARNING: NPC " + m_name + "has no behavior to run", CONSOLE_WARNING);
	}

	//CHECK IF IN DANGEROUS TILE
	Tile* onTile = g_theWorld->m_activeMap->GetTileAtLocation(m_position);
	if (onTile && onTile->IsDangerous()) {
		g_theAnimationManager->PlayAnimation(m_position, RGBA::ORANGE, m_glyph);
		DecrementHealth(1);
	}
}
void NPC::Update(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

	if (m_equipment) {
		if (m_equipment->GetWeaponInWeaponSlot()) {
			m_damage = m_equipment->GetWeaponInWeaponSlot()->GetDamageBuff();
		}
		else {
			m_damage = m_prevDamage;
		}

		if (m_equipment->GetArmorInArmorSlot()) {
			m_armor = m_equipment->GetArmorInArmorSlot()->GetArmorBuff();
		}
	}

	m_entitiesInView.clear();
	GetEntitiesInLineOfSight();
}
void NPC::Render() const {
	ASSERT_OR_DIE(nullptr != m_belongingMap, "ERROR: NPC MUST BELONG TO A MAP.");

	IntVector2 numTilesOnScreen = m_belongingMap->GetSize();
	Vector2 cellSize = Vector2(SCREEN_SIZE.x / (float)numTilesOnScreen.x, SCREEN_SIZE.y / (float)numTilesOnScreen.y);

	Vector2 positionToDrawFrom = m_position.ToVector2() * cellSize;
	Vector2 positionToDrawTo = positionToDrawFrom + cellSize;

	String glyph = "";
	glyph.push_back(m_glyph);
	float lineHeight = abs((positionToDrawFrom.y - positionToDrawTo.y) * 0.75f);

	m_squirrelFont->DrawText2D(positionToDrawFrom, glyph, lineHeight, m_color);
}

//---------------------------------------------------------------------------------------------------------------------------
//LOS & PATHFIND
//---------------------------------------------------------------------------------------------------------------------------
void NPC::GetEntitiesInLineOfSight() {
	IntVector2 npcLoc = m_position;
	vector<IntVector2> circPoints;

	//Find the circumference of the circle around the hero
	for (int radius = 5; radius < 26; radius++) {
		IntVector2 prevPoint = IntVector2(0, 0);
		for (double rad = 0; rad < pi / 4; rad += 0.04) {
			double xTrig = (double)radius * cos(rad);
			double yTrig = (double)radius * sin(rad);
			int x = (int)((double)npcLoc.x + xTrig);
			int y = (int)((double)npcLoc.y + yTrig);
			IntVector2 p(x, y);
			if (p != prevPoint) {
				circPoints.push_back(p);
				prevPoint = p;
			}
		}
	}

	//Go through each octant and draw the bressenham lines
	for (int octant = 0; octant < 8; octant++) {
		for (unsigned int i = 0; i < circPoints.size(); i++) {
			DrawBressenhamLine(octant, circPoints[i]);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void NPC::DrawBressenhamLine(int octant, IntVector2 delta) {
	IntVector2 relLoc = IntVector2(0, 0);
	IntVector2 start = m_position;
	IntVector2 end = delta;
	delta = end - start;

	int D = 2 * delta.y - delta.x;
	int y = start.y;

	IntVector2 npcLoc = m_position;

	for (int x = start.x + 1; x < end.x; x++) {

		IntVector2 rel;
		if (D > 0) {
			y += 1;
			rel = relLoc + SwitchOctant(octant, IntVector2(x, y));

			if (m_belongingMap->GetTileAtLocation(rel) && m_belongingMap->GetTileAtLocation(rel)->IsBlockingLOS()) {
				return;
			}
			else {
				Tile* currTile = m_belongingMap->GetTileAtLocation(rel);

				if (currTile && currTile->GetOccupyingAgent()) {
					m_entitiesInView.insert(currTile->GetOccupyingAgent());
				}
				if (currTile && currTile->GetInventory()) {
					m_itemsInView.insert(currTile->GetLocation());
				}
			}

			D += (2 * delta.y) - (2 * delta.x);
		}
		else {
			rel = relLoc + SwitchOctant(octant, IntVector2(x, y));

			if (m_belongingMap->GetTileAtLocation(rel) && m_belongingMap->GetTileAtLocation(rel)->IsBlockingLOS()) {
				return;
			}
			else {
				Tile* currTile = m_belongingMap->GetTileAtLocation(rel);

				if (currTile && currTile->GetOccupyingAgent()) {
					m_entitiesInView.insert(currTile->GetOccupyingAgent());
				}
				if (currTile && currTile->GetInventory()) {
					m_itemsInView.insert(currTile->GetLocation());
				}
			}

			D += (2 * delta.y);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
IntVector2 NPC::SwitchOctant(int octant, IntVector2 p) {
	IntVector2 npcLoc = m_position;

	switch (octant) {
	case 0: {
		return IntVector2(p.x, p.y);
	}
	case 1: {
		p = p - npcLoc;
		IntVector2 ret = IntVector2(p.y, p.x);
		ret = ret + npcLoc;
		return ret;
	}
	case 2: {
		p = p - npcLoc;
		IntVector2 ret(p.y, p.x);
		ret = ret + npcLoc;
		ret.y = ret.y - 2 * (ret.y - npcLoc.y);
		return ret;
	}
	case 3: {
		IntVector2 ret = p;
		ret.x = ret.x - 2 * (ret.x - npcLoc.x);
		return ret;
	}
	case 4: {
		IntVector2 ret = p;
		ret.x = ret.x - 2 * (ret.x - npcLoc.x);
		ret.y = ret.y - 2 * (ret.y - npcLoc.y);
		return ret;
	}
	case 5: {
		p = p - npcLoc;
		IntVector2 ret(p.y, p.x);
		ret = ret + npcLoc;
		ret.x = ret.x - 2 * (ret.x - npcLoc.x);
		ret.y = ret.y - 2 * (ret.y - npcLoc.y);
		return ret;
	}
	case 6: {
		p = p - npcLoc;
		IntVector2 ret(p.y, p.x);
		ret = ret + npcLoc;
		ret.x = ret.x - 2 * (ret.x - npcLoc.x);
		return ret;
	}
	case 7: {
		IntVector2 ret = p;
		ret.y = ret.y - 2 * (ret.y - npcLoc.y);
		return ret;
	}
	default: {
		return IntVector2(0, 0);
	}
	}
}

VIRTUAL bool NPC::MoveToLocation(const IntVector2& loc) {
	Tile* tileToMoveTo = m_belongingMap->GetTileAtLocation(loc);

	if (tileToMoveTo && tileToMoveTo->IsValidForAgent(m_movementProperties)) {
		m_position = loc;
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//COMBAT
//---------------------------------------------------------------------------------------------------------------------------
void NPC::NotifyAttacked(const AttackData& data) {
	Agent* attackingAgent = data.m_instigator;

	m_faction->ChangeRelationshipWithAgent(attackingAgent, FACTION_RELATIONSHIP_NEGATIVE);
}

//---------------------------------------------------------------------------------------------------------------------------
//LOGGING
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void NPC::LogDeath() {
	bool isSeen = g_theWorld->m_thePlayer->CanSeeEntity(this);
	String disp = StringUtils::Stringf("%s has died!", m_name.c_str());
	String dispFar = StringUtils::Stringf("A %s off in the distance has died!", m_name.c_str());

	(isSeen) ? (g_theMessageLog->LogMessage(disp, PRIORITY_HIGH)) : (g_theMessageLog->LogMessage(dispFar, PRIORITY_LOW));

	g_theCombatSystem->m_numMonstersKilled++;
}
VIRTUAL void NPC::LogItemsPickedUp(Inventory* inv, bool didPickUp) {
	std::vector<Item*>* items = inv->GetItemsInBag();

	if (g_theWorld->m_thePlayer->CanSeeEntity(this)) {
		if (didPickUp) {
			if (items->size() == 1) {
				String disp = StringUtils::Stringf("%s picked up a %s!", m_name.c_str(), items->at(0)->GetName().c_str());
				g_theMessageLog->LogMessage(disp, PRIORITY_LOW);
			}
			else if (items->size() > 1) {
				String disp = StringUtils::Stringf("%s picked up a stack of items!", m_name.c_str());
				g_theMessageLog->LogMessage(disp, PRIORITY_LOW);
			}
		}
	}
}

VIRTUAL void NPC::LogItemPickup(Item* item) { 
	UNREFERENCED_PARAMETER(item);
}
VIRTUAL void NPC::LogItemsPickup(Inventory* inv) { 
	UNREFERENCED_PARAMETER(inv);
}
VIRTUAL void NPC::LogItemCouldntPickup(Item* item) { 
	UNREFERENCED_PARAMETER(item);
}
VIRTUAL void NPC::LogItemDrop(Item* item) {
	g_theMessageLog->LogMessage(StringUtils::Stringf("Dropped a %s!", item->GetName().c_str()), PRIORITY_HIGH);
}
VIRTUAL void NPC::LogItemUse(Item* item) { 
	UNREFERENCED_PARAMETER(item);
}
VIRTUAL void NPC::LogEquipItem(Item* item) { 
	UNREFERENCED_PARAMETER(item);
}
VIRTUAL void NPC::LogUnequipItem(Item* item, bool couldUnequip) { 
	UNREFERENCED_PARAMETER(couldUnequip);
	UNREFERENCED_PARAMETER(item);
}


//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
void NPC::PopulateFromXMLNode(const XMLNode& templateDataNode) {
	XMLNode root = templateDataNode;

	////////////////////////////////////////////////////
	//------------------------------------------
	//ENTITY DATA
	char glyph = NPC_ITEM_DEFAULT_GLYPH;
	RGBA color = NPC_DEFAULT_COLOR;
	float moveSpeed = NPC_DEFAULT_MOVE_SPEED;
	String factionName = "";
	int armor = NPC_DEFAULT_ARMOR;

	std::vector<XMLUtils::Attribute> npcAttribs;
	XMLUtils::GetAllAttributesOfNode(templateDataNode, npcAttribs);

	for (unsigned int i = 0; i < npcAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = npcAttribs[i];
		StringUtils::ToLowerCase(currAttrib.name);

		if (currAttrib.name == "glyph") {
			glyph = XMLUtils::ParseCharFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "color") {
			color = XMLUtils::ParseHexColorFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "faction") {
			factionName = currAttrib.value;
		}
		else if (currAttrib.name == "movespeed") {
			moveSpeed = (float)XMLUtils::ParseFloatFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "startingArmor") {
			armor = XMLUtils::ParseIntFromXML(currAttrib.value);
		}
	}

	m_glyph = glyph;
	m_color = color;
	m_faction = Faction::CreateFactionByName(factionName);
	m_moveSpeed = moveSpeed;
	m_health = m_maxHealth;
	m_armor = armor;

	//PARSE CHILDREN
	for (int i = 0; i < root.nChildNode(); i++) {
		XMLNode currNode = root.getChildNode(i);

		String nodeName = currNode.getName();
		StringUtils::ToLowerCase(nodeName);

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE FACTION OVERRIDES
		if (nodeName == "faction") {
			ParseFactionOverride(currNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE BEHAVIORS
		else if (nodeName == "behaviors") {
			ParseBehaviors(currNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE INVENTORY / EQUIPMENT
		else if (nodeName == "inventory") {
			ParseInventory(currNode);
		}

		else if (nodeName == "equipment") {
			ParseEquipment(currNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE MOVEMENT PROPERTIES
		else if (nodeName == "movementproperties") {
			ParseMovementProperties(currNode);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void NPC::ParseFactionOverride(const XMLNode& factionOverrideNode) {

	if (m_faction) {
		delete m_faction;
		m_faction = nullptr;
	}

	std::vector<XMLUtils::Attribute> factionOverrideAttribs;
	XMLUtils::GetAllAttributesOfNode(factionOverrideNode, factionOverrideAttribs);

	for (unsigned int i = 0; i < factionOverrideAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = factionOverrideAttribs[i];
		String currAttribName = currAttrib.name;
		String currAttribVal = currAttrib.value;
		StringUtils::ToLowerCase(currAttribName);
		StringUtils::ToLowerCase(currAttribVal);

		eFactionRelationship currRelationship = NUM_FACTION_RELATIONSHIPS;

		if (currAttribName == "name") {
			m_faction = Faction::CreateFactionByName(currAttribVal);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE THE RELATIONSHIP
		else if (currAttrib.name == "loves") {
			currRelationship = FACTION_RELATIONSHIP_LOVES;
		}
		else if (currAttrib.name == "likes") {
			currRelationship = FACTION_RELATIONSHIP_LIKES;
		}
		else if (currAttrib.name == "dislikes") {
			currRelationship = FACTION_RELATIONSHIP_DISLIKES;
		}
		else if (currAttrib.name == "hates") {
			currRelationship = FACTION_RELATIONSHIP_HATES;
		}

		if (currRelationship != NUM_FACTION_RELATIONSHIPS) {
			std::vector<String> factionsToChange;
			StringUtils::TokenizeBuffer(currAttrib.value, ',', factionsToChange);

			for (unsigned int j = 0; j < factionsToChange.size(); j++) {
				String& currFactionToChange = factionsToChange[j];
				StringUtils::ToLowerCase(currFactionToChange);

				if (m_faction) {
					m_faction->OverrideFactionRelationship(currFactionToChange, currRelationship);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void NPC::ParseBehaviors(const XMLNode& behaviorsNode) {
	for (int i = 0; i < behaviorsNode.nChildNode(); i++) {
		XMLNode behavior = behaviorsNode.getChildNode(i);
		String bName = behavior.getName();

		BehaviorData* bData = BehaviorDataRegistration::CreateBDDataByName(bName, behavior);
		m_behaviors.push_back(BehaviorRegistration::CreateBehaviorByName(bName, bData));
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void NPC::ParseInventory(const XMLNode& invNode) {

	for (int i = 0; i < invNode.nChildNode(); i++) {

		XMLNode currNode = invNode.getChildNode(i);
		String currNodeName = currNode.getName();

		if (currNodeName == "Item") {
			
			std::vector<XMLUtils::Attribute> itemAttribs;
			XMLUtils::GetAllAttributesOfNode(currNode, itemAttribs);

			for (unsigned int j = 0; j < itemAttribs.size(); j++) {
				XMLUtils::Attribute& currAttrib = itemAttribs[j];
				String currAttribName = currAttrib.name;
				String currAttribVal = currAttrib.value;
				StringUtils::ToLowerCase(currAttribName);
				//StringUtils::ToLowerCase(currAttribVal);

				if (currAttribName == "name") {
					Item* newItem = ItemFactory::GetFactoryByName(currAttribVal)->Create(g_theWorld->m_activeMap, IntVector2(-1, -1));

					if (newItem) {
						m_inventory->PutItemInBag(newItem);
					}
					else {
						DebuggerPrintf("ERROR: XML Parser error in NPC parsing inventory items.");
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void NPC::ParseEquipment(const XMLNode& equipmentNode) {
	std::vector<XMLUtils::Attribute> itemAttribs;
	XMLUtils::GetAllAttributesOfNode(equipmentNode, itemAttribs);

	for (unsigned int i = 0; i < itemAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = itemAttribs[i];
		String currAttribName = currAttrib.name;
		String currAttribVal = currAttrib.value;
		StringUtils::ToLowerCase(currAttribName);
		StringUtils::ToLowerCase(currAttribVal);

		if (currAttribName == "name") {
			Item* newItem = ItemFactory::GetFactoryByName(currAttribVal)->Create(g_theWorld->m_activeMap, IntVector2(-1, -1));

			if (newItem) {
				switch (newItem->GetType()) {
				case ITEM_WEAPON:
					g_theWorld->AddItemToEntityList(newItem);
					m_equipment->EquipWeapon(newItem);
					break;
				case ITEM_ARMOR:
					g_theWorld->AddItemToEntityList(newItem);
					m_equipment->EquipArmor(newItem);
					break;
				default:
					DebuggerPrintf("ERROR: Cannot equip a potion or non-armor / weapon item in NPC equipment slots.");
					break;
				}
			}
			else {
				DebuggerPrintf("ERROR: XML Parser error in NPC parsing inventory items.");
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void NPC::ParseMovementProperties(const XMLNode& movementProperties) {
	m_movementProperties = 0;

	for (int i = 0; i < movementProperties.nChildNode(); i++) {
		XMLNode currNode = movementProperties.getChildNode(i);

		String currNodeName = currNode.getName();

		if (currNodeName == "BlockedByLava") {
			m_movementProperties |= BLOCKED_BY_LAVA;
		}
		else if (currNodeName == "SlowedByLava") {
			m_movementProperties |= SLOWED_BY_LAVA;
		}
		else if (currNodeName == "BlockedByWater") {
			m_movementProperties |= BLOCKED_BY_WATER;
		}
		else if (currNodeName == "SlowedByWater") {
			m_movementProperties |= SLOWED_BY_WATER;
		}
		else if (currNodeName == "BlockedByWalls") {
			m_movementProperties |= BLOCKED_BY_WALLS;
		}
		else if (currNodeName == "BlockedByAgents") {
			m_movementProperties |= BLOCKED_BY_AGENTS;
		}
	}

	//String debug = BitUtils::GetBitsAsString(m_movementProperties);
}

//---------------------------------------------------------------------------------------------------------------------------
//UTILITY
//---------------------------------------------------------------------------------------------------------------------------
void NPC::GetAdjacentAgentsWithinManhattanDistance(int dist, std::vector<Agent*>& outVec) const {

	for (std::set<Entity*>::iterator eIter = m_entitiesInView.begin(); eIter != m_entitiesInView.end(); eIter++) {
		Entity* currEntity = *eIter;

		if (currEntity->IsAgent()
			&& abs(currEntity->GetPosition().x - GetPosition().x) <= dist
			&& abs(currEntity->GetPosition().y - GetPosition().y) <= dist)
		{
			outVec.push_back(dynamic_cast<Agent*>(currEntity));
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVING LOADING
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL XMLNode NPC::SaveToXML() const {
	XMLNode npcBlueprint;
	npcBlueprint = npcBlueprint.createXMLTopNode("NPCBlueprint");

	////////////////////////////////////////////////////
	//------------------------------------------
	//ENTITY PROPERTIES
	npcBlueprint.addAttribute("name", m_name.c_str());
	npcBlueprint.addAttribute("faction", m_faction->GetName().c_str());
	npcBlueprint.addAttribute("health", std::to_string(m_health).c_str());
	npcBlueprint.addAttribute("maxHealth", std::to_string(m_maxHealth).c_str());
	npcBlueprint.addAttribute("glyph", StringUtils::ToString(m_glyph).c_str());
	npcBlueprint.addAttribute("color", m_color.ToString().c_str());
	npcBlueprint.addAttribute("bgColor", m_backgroundColor.ToString().c_str());
	npcBlueprint.addAttribute("savedID", std::to_string(m_id).c_str());
	npcBlueprint.addAttribute("position", m_position.ToString().c_str());
	npcBlueprint.addAttribute("armor", std::to_string(m_armor).c_str());
	npcBlueprint.addAttribute("moveSpeed", std::to_string(m_moveSpeed).c_str());

	////////////////////////////////////////////////////
	//------------------------------------------
	//BEHAVIORS
	XMLNode behaviors;
	behaviors = behaviors.createXMLTopNode("Behaviors");
	for (unsigned int i = 0; i < m_behaviors.size(); i++) {
		XMLNode behaviorNode;
		behaviorNode = m_behaviors[i]->SaveToXML();
		behaviors.addChild(behaviorNode);
	}
	npcBlueprint.addChild(behaviors);

	////////////////////////////////////////////////////
	//------------------------------------------
	//AGENT RELATIONSHIPS
	XMLNode aRelNode = aRelNode.createXMLTopNode("AgentRelationships");

	std::map<uint, eFactionRelationship> agentRelationships = m_faction->GetAgentRelationships();

	for (std::map<uint, eFactionRelationship>::const_iterator it = agentRelationships.begin(); it != agentRelationships.end(); ++it) {

		XMLNode aNode = aNode.createXMLTopNode("Agent");
		aNode.addAttribute("savedID", std::to_string(it->first).c_str());
		aNode.addAttribute("relationship", std::to_string(it->second).c_str());
		aRelNode.addChild(aNode);
	}

	npcBlueprint.addChild(aRelNode);

	////////////////////////////////////////////////////
	//------------------------------------------
	//INVENTORY EQUIPMENT
	if (m_inventory) {
		npcBlueprint.addChild(m_inventory->SaveToXML(IntVector2(-1,-1)));
	}
	if (m_equipment) {
		npcBlueprint.addChild(m_equipment->SaveToXML());
	}

	////////////////////////////////////////////////////
	//------------------------------------------
	//MOVEMENT PROPERTIES
	XMLNode movementProperties;
	movementProperties = movementProperties.createXMLTopNode("MovementProperties");

	if (BitUtils::IsBitSet(m_movementProperties, BLOCKED_BY_LAVA)) {
		XMLNode mNode = mNode.createXMLTopNode("BlockedByLava");
		movementProperties.addChild(mNode);
	}
	if (BitUtils::IsBitSet(m_movementProperties, SLOWED_BY_LAVA)) {
		XMLNode mNode = mNode.createXMLTopNode("SlowedByLava");
		movementProperties.addChild(mNode);
	}
	if (BitUtils::IsBitSet(m_movementProperties, BLOCKED_BY_WATER)) {
		XMLNode mNode = mNode.createXMLTopNode("BlockedByWater");
		movementProperties.addChild(mNode);
	}
	if (BitUtils::IsBitSet(m_movementProperties, SLOWED_BY_WATER)) {
		XMLNode mNode = mNode.createXMLTopNode("SlowedByWater");
		movementProperties.addChild(mNode);
	}
	if (BitUtils::IsBitSet(m_movementProperties, BLOCKED_BY_AGENTS)) {
		XMLNode mNode = mNode.createXMLTopNode("BlockedByAgents");
		movementProperties.addChild(mNode);
	}
	if (BitUtils::IsBitSet(m_movementProperties, BLOCKED_BY_WALLS)) {
		XMLNode mNode = mNode.createXMLTopNode("BlockedByWalls");
		movementProperties.addChild(mNode);
	}

	npcBlueprint.addChild(movementProperties);

	return npcBlueprint;
}

//---------------------------------------------------------------------------------------------------------------------------
//LOADING
//---------------------------------------------------------------------------------------------------------------------------
void NPC::LoadFromXML(const XMLNode& node) {

	m_currTargetedEnemy = nullptr;
	m_currTargetedAlly = nullptr;
	m_currTargetedItem = nullptr;
	m_currPathFind = nullptr;

	m_inventory = new Inventory();
	m_equipment = new Equipment();
	m_belongingMap = g_theWorld->m_activeMap;

	std::vector<XMLUtils::Attribute> itemAttribs;
	XMLUtils::GetAllAttributesOfNode(node, itemAttribs);

	for (unsigned int i = 0; i < itemAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = itemAttribs[i];
		String currAttribName = currAttrib.name;
		String currAttribVal = currAttrib.value;
		StringUtils::ToLowerCase(currAttribName);

		if (currAttribName == "name") {
			m_name = currAttribVal;
		}
		else if (currAttribName == "faction") {
			StringUtils::ToLowerCase(currAttribVal);
			m_faction = Faction::CreateFactionByName(currAttribVal);
		}
		else if (currAttribName == "health") {
			m_health = XMLUtils::ParseIntFromXML(currAttribVal);
		}
		else if (currAttribName == "maxhealth") {
			m_maxHealth = XMLUtils::ParseIntFromXML(currAttribVal);
		}
		else if (currAttribName == "glyph") {
			m_glyph = XMLUtils::ParseCharFromXML(currAttribVal);
		}
		else if (currAttribName == "color") {
			m_color = XMLUtils::ParseFloatColorFromXML(currAttribVal);
		}
		else if (currAttribName == "bgcolor") {
			m_backgroundColor = XMLUtils::ParseFloatColorFromXML(currAttribVal);
		}
		else if (currAttribName == "savedid") {
			m_savedID = XMLUtils::ParseIntFromXML(currAttribVal);
		}
		else if (currAttribName == "position") {
			m_position = XMLUtils::ParseIntVector2FromXML(currAttribVal);
		}
		else if (currAttribName == "armor") {
			m_armor = XMLUtils::ParseIntFromXML(currAttribVal);
		}
		else if (currAttribName == "movespeed") {
			m_moveSpeed = XMLUtils::ParseFloatFromXML(currAttribVal);
		}
	}

	if (g_theWorld->m_activeMap) {
		Tile* tileToPlaceOn = g_theWorld->m_activeMap->GetTileAtLocation(m_position);
		tileToPlaceOn->SetOccupyingAgent(this);
	}
	else {
		DebuggerPrintf("ERROR: Map trying to place agent on is null. Shouldn't happen.");
	}

	for (int i = 0; i < node.nChildNode(); i++) {
		XMLNode currNode = node.getChildNode(i);
		String currNodeName = currNode.getName();

		if (currNodeName == "Behaviors") {
			ParseBehaviors(currNode);
		}
		if (currNodeName == "AgentRelationships") {
			ParseLoadedAgentRelationships(currNode);
		}
		if (currNodeName == "Inventory") {
			ParseInventory(currNode);
		}
		if (currNodeName == "Equipment") {
			ParseEquipment(currNode);
		}
		if (currNodeName == "MovementProperties") {
			ParseMovementProperties(currNode);
		}
	}
}

void NPC::ParseLoadedAgentRelationships(const XMLNode& node) {

	for (int i = 0; i < node.nChildNode(); i++) {
		
		XMLNode currNode = node.getChildNode(i);
		uint savedID = (uint)XMLUtils::ParseIntFromXML(currNode.getAttribute(0).lpszValue);
		eFactionRelationship relationship = (eFactionRelationship)XMLUtils::ParseIntFromXML(currNode.getAttribute(1).lpszValue);

		m_loadedAgentRelationships.push_back(std::pair<uint, eFactionRelationship>(savedID, relationship));
	}
}

VIRTUAL void NPC::ResolveEntityPointers() {
	if (m_loadedAgentRelationships.size() == 0) {
		return;
	}

	for (unsigned int i = 0; i < m_loadedAgentRelationships.size(); i++) {
		uint entityID = g_theWorld->GetEntityIDFromSavedID(m_loadedAgentRelationships[i].first);

		m_agentRelationships.insert(std::pair<uint, eFactionRelationship>(entityID, m_loadedAgentRelationships[i].second));
	}
}