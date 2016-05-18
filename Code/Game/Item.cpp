#include "Game/Item.hpp"
#include "Game/Map.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Item::Item(const XMLNode& itemNode) {
	PopulateFromXMLNode(itemNode);

	m_id = Entity::s_entityIDs++;
}
Item::Item(Item* templateItem) {
	m_type = templateItem->GetType();
	m_damageBuff = templateItem->GetDamageBuff();
	m_armorBuff = templateItem->GetArmorBuff();
	m_healAmount = templateItem->GetHealAmount();

	m_position = templateItem->GetPosition();
	m_glyph = templateItem->GetGlyph();
	m_color = templateItem->GetColor();
	m_backgroundColor = templateItem->GetBackgroundColor();
	m_name = templateItem->GetName();
	m_belongingMap = templateItem->GetBelongingMap();
	
	m_id = Entity::s_entityIDs++;
}

void Item::PopulateFromXMLNode(const XMLNode& itemNode) {

	XMLNode root = itemNode;

	////////////////////////////////////////////////////
	//------------------------------------------
	//ENTITY DATA
	m_name = "DEFAULT_NAME";
	String type = "";
	m_damageBuff = RangeInt(0, 0);
	m_armorBuff = 0;
	m_healAmount = 0;

	std::vector<XMLUtils::Attribute> npcAttribs;
	XMLUtils::GetAllAttributesOfNode(itemNode, npcAttribs);

	for (unsigned int i = 0; i < npcAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = npcAttribs[i];
		StringUtils::ToLowerCase(currAttrib.name);

		if (currAttrib.name == "name") {
			m_name = currAttrib.value;
		}
		else if (currAttrib.name == "type") {
			type = currAttrib.value;
			StringUtils::ToLowerCase(type);
		}
		else if (currAttrib.name == "amount") {
			m_armorBuff = XMLUtils::ParseIntFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "damage") {
			m_damageBuff = XMLUtils::ParseRangeIntFromXML(currAttrib.value);
		}
		else if (currAttrib.name == "healamount") {
			m_healAmount = XMLUtils::ParseIntFromXML(currAttrib.value);
		}
	}

	if (type == "weapon") {
		m_type = ITEM_WEAPON;
		m_glyph = '(';
	}
	else if (type == "armor") {
		m_type = ITEM_ARMOR;
		m_glyph = '[';
	}
	else if (type == "potion") {
		m_type = ITEM_POTION;
		m_glyph = '\'';
	}
	else {
		m_type = ITEM_NUM_ITEM_TYPES;
		DebuggerPrintf("WARNING: Invalid item parsed from XML.");
	}

	m_color = RGBA::WHITE;
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void Item::Update(float deltaSeconds) { 
	UNREFERENCED_PARAMETER(deltaSeconds);
}
VIRTUAL void Item::Render() const {
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
VIRTUAL void Item::Run() { }

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
String Item::BonusToString() const {
	switch (m_type) {
	case ITEM_WEAPON:
		return StringUtils::Stringf("Damage: %s", m_damageBuff.ToString().c_str());
	case ITEM_ARMOR:
		return StringUtils::Stringf("Armor: %i", m_armorBuff);
	case ITEM_POTION:
		return StringUtils::Stringf("Heal Amount: %i", m_healAmount);
	default:
		return "";
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//USE
//---------------------------------------------------------------------------------------------------------------------------
void Item::ApplyBuffs(Agent* agentToApplyTo) {
	switch (m_type) {
	case ITEM_WEAPON:
		agentToApplyTo->SetPrevDamage(agentToApplyTo->GetDamage());
		agentToApplyTo->SetDamage(m_damageBuff);
		break;
	case ITEM_ARMOR:
		agentToApplyTo->SetArmor(m_armorBuff);
		break;
	default:
		break;
	}
}
void Item::RemoveBuffs(Agent* agentToApplyTo) {
	switch (m_type) {
	case ITEM_WEAPON:
		agentToApplyTo->SetDamage(agentToApplyTo->GetPrevDamage());
		break;
	case ITEM_ARMOR:
		agentToApplyTo->SetArmor(0);
		break;
	default:
		break;
	}
}
void Item::Use(Agent* agentToApplyTo) {
	if (m_type == ITEM_POTION) {
		agentToApplyTo->IncrementHealth(m_healAmount);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVING LOADING
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL XMLNode Item::SaveToXML() const {

	XMLNode itemBlueprint;
	return itemBlueprint;
}

//---------------------------------------------------------------------------------------------------------------------------
//LOADING
//---------------------------------------------------------------------------------------------------------------------------
void Item::LoadFromXML(const XMLNode& node) {
	UNREFERENCED_PARAMETER(node);
}

VIRTUAL void Item::ResolveEntityPointers() { }