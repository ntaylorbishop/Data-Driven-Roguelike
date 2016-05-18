#include "Game/Player.hpp"
#include "Game/TheWorld.hpp"
#include "Game/PathFinder.hpp"
#include "Game/TheMessageLog.hpp"
#include "Game/Equipment.hpp"
#include "Game/TheAnimationManager.hpp"

STATIC RangeInt Player::s_playerDamage = RangeInt(0, 0);
STATIC int Player::s_playerMaxHealth = 0;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS INIT
//---------------------------------------------------------------------------------------------------------------------------
Player::Player(char glyph, const RGBA& color, const RGBA& bgColor, Map* map)
	: Agent(IntVector2(-1, -1), glyph, color, bgColor, "Player", map)
	, m_squirrelFont(nullptr)
	, m_playerPathFind(nullptr)
	, m_isReadyToUpdate(false)
{  
	Agent::Initialize(s_playerMaxHealth, s_playerMaxHealth, s_playerDamage, 1.f);

	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");
	m_position = m_belongingMap->FindRandomValidLocation();
	m_playerPathFind = new PathFinder();

	m_faction = Faction::CreateFactionByName("Player");

	m_inventory = new Inventory(true);
	m_equipment = new Equipment();
}
Player::Player(const XMLNode& playerDataNode) 
	: Agent(IntVector2(-1, -1), '@', RGBA::WHITE, RGBA::BLACK, "Player", nullptr)
{
	LoadFromXML(playerDataNode);
}

STATIC void Player::InitializePlayerData() {
	String filePath = "Data/Player/Player.xml";

	XMLNode root = XMLNode::parseFile(filePath.c_str());

	root = root.getChildNode(0);
	root = root.getChildNode(0);

	ASSERT_OR_DIE(root.nAttribute() == 2, "ERROR: PLAYER XML PARSE ERROR");

	String healthAttrib = root.getAttribute(0).lpszValue;
	String baseDamageAttrib = root.getAttribute(1).lpszValue;

	s_playerMaxHealth = XMLUtils::ParseIntFromXML(healthAttrib);
	s_playerDamage = XMLUtils::ParseRangeIntFromXML(baseDamageAttrib);
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void Player::Run() {
	m_position = m_nextLoc;

	Tile* onTile = g_theWorld->m_activeMap->GetTileAtLocation(m_position);

	if (onTile && onTile->IsDangerous()) {
		g_theMessageLog->LogMessage("Your feet burn from the lava! You take 1 damage.", PRIORITY_HIGH);
		g_theAnimationManager->PlayAnimation(m_position, RGBA::ORANGE, m_glyph);
		DecrementHealth(1);
	}
}
void Player::Update(float deltaSeconds) {
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

	m_isReadyToUpdate = false;
	HandleInput();
	HandleFeatureToggle();
}

void Player::Render() const {
	if (nullptr != m_belongingMap && m_isAlive) {
		IntVector2 numTilesOnScreen = m_belongingMap->GetSize();
		Vector2 cellSize = Vector2(SCREEN_SIZE.x / (float)numTilesOnScreen.x, SCREEN_SIZE.y / (float)numTilesOnScreen.y);

		Vector2 positionToDrawFrom = m_position.ToVector2() * cellSize;
		Vector2 positionToDrawTo = positionToDrawFrom + cellSize;

		String glyph = "";
		glyph.push_back(m_glyph);
		float lineHeight = abs((positionToDrawFrom.y - positionToDrawTo.y) * 0.75f);

		m_squirrelFont->DrawText2D(positionToDrawFrom, glyph, lineHeight, m_color);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//INPUT
//---------------------------------------------------------------------------------------------------------------------------
void Player::HandleInput() {
	m_nextLoc = m_position;
	bool triedToMove = false;

	//LEFT
	if (g_theInputSystem->GetKeyDown('H') || g_theInputSystem->GetKeyDown(NUM_KEY_4)) {
		m_nextLoc = m_nextLoc + IntVector2(-1, 0);
		g_theWorld->DeleteCurrPathFinder();
		triedToMove = true;
	}
	//DOWN
	else if (g_theInputSystem->GetKeyDown('J') || g_theInputSystem->GetKeyDown(NUM_KEY_2)) {
		m_nextLoc = m_nextLoc + IntVector2(0, -1);
		g_theWorld->DeleteCurrPathFinder();
		triedToMove = true;
	}
	//UP
	else if (g_theInputSystem->GetKeyDown('K') || g_theInputSystem->GetKeyDown(NUM_KEY_8)) {
		m_nextLoc = m_nextLoc + IntVector2(0, 1);
		g_theWorld->DeleteCurrPathFinder();
		triedToMove = true;
	}
	//RIGHT
	else if (g_theInputSystem->GetKeyDown('L') || g_theInputSystem->GetKeyDown(NUM_KEY_6)) {
		m_nextLoc = m_nextLoc + IntVector2(1, 0);
		g_theWorld->DeleteCurrPathFinder();
		triedToMove = true;
	}
	//NORTHWEST
	else if (g_theInputSystem->GetKeyDown('Y') || g_theInputSystem->GetKeyDown(NUM_KEY_7)) {
		m_nextLoc = m_nextLoc + IntVector2(-1, 1);
		g_theWorld->DeleteCurrPathFinder();
		triedToMove = true;
	}
	//NORTHEAST
	else if (g_theInputSystem->GetKeyDown('U') || g_theInputSystem->GetKeyDown(NUM_KEY_9)) {
		m_nextLoc = m_nextLoc + IntVector2(1, 1);
		g_theWorld->DeleteCurrPathFinder();
		triedToMove = true;
	}
	//SOUTHWEST
	else if (g_theInputSystem->GetKeyDown('B') || g_theInputSystem->GetKeyDown(NUM_KEY_1)) {
		m_nextLoc = m_nextLoc + IntVector2(-1, -1);
		g_theWorld->DeleteCurrPathFinder();
		triedToMove = true;
	}
	//SOUTHEAST
	else if (g_theInputSystem->GetKeyDown('N') || g_theInputSystem->GetKeyDown(NUM_KEY_3)) {
		m_nextLoc = m_nextLoc + IntVector2(1, -1);
		g_theWorld->DeleteCurrPathFinder();
		triedToMove = true;
	}

	if (g_theInputSystem->GetKeyDown('P')) {
		g_theWorld->CreateOrStepPathFind();
	}

	m_tileToMoveTo = m_belongingMap->GetTileAtLocation(m_nextLoc);

	if (g_theInputSystem->GetKeyDown(VK_OEM_COMMA)) {
		m_tileToMoveTo = m_belongingMap->GetTileAtLocation(m_nextLoc);

		if (m_tileToMoveTo->GetInventory()) {
			PickUpItems(m_tileToMoveTo);
		}
	}

	if (g_theInputSystem->GetKeyDown('Z')) {
		DEBUG_RENDER_PATH_FIND = !DEBUG_RENDER_PATH_FIND;
	}

	if (m_tileToMoveTo && m_nextLoc != m_position && !m_tileToMoveTo->IsBlockingPathing()) {

		if (m_tileToMoveTo->GetOccupyingAgent()) {
			m_nextLoc = m_position;
			g_theCombatSystem->Attack(this, m_tileToMoveTo->GetOccupyingAgent());
		}
		else if (m_tileToMoveTo->GetInventory()) {
			LogItemsPickedUp(m_tileToMoveTo->GetInventory(), false);
		}

		m_isReadyToUpdate = true;
		g_theCombatSystem->IncrementTurnCount();
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void Player::HandleFeatureToggle() {
	if (g_theInputSystem->GetMouseBtn(MOUSE_LEFT)) {
		Tile* hoveredTile = g_theWorld->m_currHoveredTile;

		if (hoveredTile && hoveredTile->GetFeature() && Map::IsLocAdjacentToLoc(m_position, hoveredTile->GetLocation())) {
			hoveredTile->GetFeature()->Toggle();
		}
	}

	if (g_theInputSystem->GetKeyDown('A')) {
		const int MAX_SURROUNDING_TILES = 8;

		Tile* tilesToCheck[MAX_SURROUNDING_TILES];

		tilesToCheck[0] = m_belongingMap->GetTileAtLocation(m_position + NORTH);
		tilesToCheck[1] = m_belongingMap->GetTileAtLocation(m_position + NORTHEAST);
		tilesToCheck[2] = m_belongingMap->GetTileAtLocation(m_position + EAST);
		tilesToCheck[3] = m_belongingMap->GetTileAtLocation(m_position + SOUTHEAST);
		tilesToCheck[4] = m_belongingMap->GetTileAtLocation(m_position + SOUTH);
		tilesToCheck[5] = m_belongingMap->GetTileAtLocation(m_position + SOUTHWEST);
		tilesToCheck[6] = m_belongingMap->GetTileAtLocation(m_position + WEST);
		tilesToCheck[7] = m_belongingMap->GetTileAtLocation(m_position + NORTHWEST);

		for (int i = 0; i < MAX_SURROUNDING_TILES; i++) {
			if (tilesToCheck && tilesToCheck[i]->GetFeature()) {
				tilesToCheck[i]->GetFeature()->Toggle();
			}
		}
	}
}
 
bool Player::CanSeeEntity(Entity* entity) {
	bool canSeeEntity = false;

	for (unsigned int i = 0; i < m_visibleEntities.size(); i++) {
		if (m_visibleEntities[i]->GetEntityID() == entity->GetEntityID()) {
			canSeeEntity = true;
		}
	}

	return canSeeEntity;
}

//---------------------------------------------------------------------------------------------------------------------------
//LOGGING
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void Player::LogDeath() {
	String disp = "You have died!";
	g_theMessageLog->LogMessage(disp, PRIORITY_HIGH);
}
VIRTUAL void Player::LogItemsPickedUp(Inventory* inv, bool didPickUp) {
	std::vector<Item*>* items = inv->GetItemsInBag();

	if (!didPickUp) {
		if (items->size() == 1) {
			String disp = StringUtils::Stringf("You find a %s here.", items->at(0)->GetName().c_str());
			g_theMessageLog->LogMessage(disp, PRIORITY_HIGH);
		}
		else if (items->size() > 1) {
			String disp = "You find a stack of items here.";
			g_theMessageLog->LogMessage(disp, PRIORITY_HIGH);
		}
	}
	else {
		if (items->size() == 1) {
			String disp = StringUtils::Stringf("You picked up a %s.", items->at(0)->GetName().c_str());
			g_theMessageLog->LogMessage(disp, PRIORITY_HIGH);
		}
		else if (items->size() > 1) {
			String disp = "You picked up a stack of items.";
			g_theMessageLog->LogMessage(disp, PRIORITY_HIGH);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//ITEM HANDLING
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void Player::LogItemPickup(Item* item) {
	g_theMessageLog->LogMessage(StringUtils::Stringf("Picked up a %s.", item->GetName().c_str()), PRIORITY_HIGH);
}
VIRTUAL void Player::LogItemsPickup(Inventory* inv) {
	UNREFERENCED_PARAMETER(inv);
	g_theMessageLog->LogMessage(StringUtils::Stringf("Picked up a stack of items."), PRIORITY_HIGH);
}
VIRTUAL void Player::LogItemCouldntPickup(Item* item) {
	g_theMessageLog->LogMessage(StringUtils::Stringf("Couldn't pick up %s! Inventory full.", item->GetName().c_str()), PRIORITY_HIGH);
}
VIRTUAL void Player::LogItemDrop(Item* item) {
	g_theMessageLog->LogMessage(StringUtils::Stringf("Dropped a %s.", item->GetName().c_str()), PRIORITY_HIGH);
}
VIRTUAL void Player::LogItemUse(Item* item) {
	g_theMessageLog->LogMessage(StringUtils::Stringf("Used a %s.", item->GetName().c_str()), PRIORITY_HIGH);
}
VIRTUAL void Player::LogEquipItem(Item* item) {
	g_theMessageLog->LogMessage(StringUtils::Stringf("Equipped a %s.", item->GetName().c_str()), PRIORITY_HIGH);
}
VIRTUAL void Player::LogUnequipItem(Item* item, bool couldUnequip) {
	if (couldUnequip) {
		g_theMessageLog->LogMessage(StringUtils::Stringf("Unequipped a %s.", item->GetName().c_str()), PRIORITY_HIGH);
	}
	else {
		g_theMessageLog->LogMessage(StringUtils::Stringf("Couldn't unequip %s! Inventory is full.", item->GetName().c_str()), PRIORITY_HIGH);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVE LOADING
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL XMLNode Player::SaveToXML() const {
	XMLNode playerData;
	playerData = playerData.createXMLTopNode("PlayerData");

	playerData.addAttribute("health", std::to_string(m_health).c_str());
	playerData.addAttribute("maxHealth", std::to_string(m_maxHealth).c_str());
	playerData.addAttribute("damage", m_damage.ToString().c_str());
	playerData.addAttribute("armor", std::to_string(m_armor).c_str());
	playerData.addAttribute("position", m_position.ToString().c_str());
	playerData.addAttribute("savedID", std::to_string(m_savedID).c_str());

	XMLNode invData = m_inventory->SaveToXML(IntVector2(-1, -1));
	XMLNode eData = m_equipment->SaveToXML();

	playerData.addChild(invData);
	playerData.addChild(eData);

	return playerData;
}
void Player::LoadFromXML(const XMLNode& playerDataNode) {

	std::vector<XMLUtils::Attribute> npcAttribs;
	XMLUtils::GetAllAttributesOfNode(playerDataNode, npcAttribs);


	for (unsigned int i = 0; i < npcAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = npcAttribs[i];
		StringUtils::ToLowerCase(currAttrib.name);

		if (currAttrib.name == "health") {
			m_health = XMLUtils::ParseIntFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "maxhealth") {
			m_maxHealth = XMLUtils::ParseIntFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "damage") {
			m_damage = XMLUtils::ParseRangeIntFromXML(currAttrib.value);
			m_prevDamage = m_damage;
		}
		else if (currAttrib.name == "armor") {
			m_armor = XMLUtils::ParseIntFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "position") {
			m_position = XMLUtils::ParseIntVector2FromXML(currAttrib.value);
		}
		else if (currAttrib.name == "savedID") {
			m_savedID = XMLUtils::ParseIntFromXML(currAttrib.value);
		}
	}

	m_id = Entity::s_entityIDs++;
	m_belongingMap = g_theWorld->m_activeMap;
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");
	m_playerPathFind = new PathFinder();

	m_faction = Faction::CreateFactionByName("Player");

	m_inventory = new Inventory(true);
	m_equipment = new Equipment();
	
	for (int i = 0; i < playerDataNode.nChildNode(); i++) {

		XMLNode currNode = playerDataNode.getChildNode(i);
		String currNodeName = currNode.getName();

		if (currNodeName == "Inventory") {
			m_inventory->LoadFromXML(currNode);
		}
		else if (currNodeName == "Equipment") {
			m_equipment->LoadFromXML(currNode);
		}
	}
}