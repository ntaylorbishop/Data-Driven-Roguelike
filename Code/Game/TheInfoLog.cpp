#include "Game/TheInfoLog.hpp"
#include "Game/TheMessageLog.hpp"
#include "Game/TheUI.hpp"
#include "Game/TheWorld.hpp"

TheInfoLog* g_theInfoLog = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
TheInfoLog::TheInfoLog() 
	: m_healthBarQuad(AABB2())
	, m_saveAge(0.f)
	, m_saveColor(RGBA::WHITE)
	, m_couldSave(false)
{
	Vector2 healthBarQuadStart = INFO_LOG_START_LOC + Vector2(10.f, -10.f);
	Vector2 healthBarQuadEnd = Vector2(INFO_LOG_END_LOC.x - 10.f, healthBarQuadStart.y - 20.f);
	m_healthBarQuad = AABB2(healthBarQuadStart, healthBarQuadEnd);
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheInfoLog::Update(float deltaSeconds) {

	m_saveAge -= deltaSeconds;
}
void TheInfoLog::Render() const {
	//g_theRenderer->DrawAABB2(AABB2(INFO_LOG_START_LOC, INFO_LOG_END_LOC), RGBA(0.f, 0.f, 0.f, 1.f), PRIMITIVE_QUADS);
	g_theRenderer->DrawAABB2(m_healthBarQuad, RGBA(0x0b, 0x77, 0x00, 0.8f), PRIMITIVE_QUADS);

	Player* player = g_theWorld->m_thePlayer;
	if (player) {
		String playerHealth = "Health: " + std::to_string(player->GetHealth()) + "/" + std::to_string(player->GetMaxHealth());
		String playerAttack = "Damage: " + player->GetDamage().ToString();
		String playerArmor = "Armor: " + std::to_string(player->GetArmor());

		g_theUI->DrawTextAtLocation(m_healthBarQuad.mins + Vector2(10.f, -40.f), playerHealth, 0.3f, RGBA(0.9f, 0.9f, 0.9f, 1.f));
		g_theUI->DrawTextAtLocation(m_healthBarQuad.mins + Vector2(10.f, -60.f), playerAttack, 0.3f, RGBA(0.9f, 0.9f, 0.9f, 1.f));
		g_theUI->DrawTextAtLocation(m_healthBarQuad.mins + Vector2(10.f, -80.f), playerArmor, 0.3f, RGBA(0.9f, 0.9f, 0.9f, 1.f));
	}

	RenderHoverLog();

	if (m_saveAge > 0.f) {
		RenderSaveAnimation();
	}
}

void TheInfoLog::RenderHoverLog() const {
	Tile*& hoveredTile = g_theWorld->m_currHoveredTile;

	if (hoveredTile) {
		if (hoveredTile->GetOccupyingAgent()) {
			RenderNPCData(hoveredTile->GetOccupyingAgent());
		}
		else if (hoveredTile->GetInventory()) {
			RenderItemData(hoveredTile->GetInventory());
		}
		else if (hoveredTile->GetFeature()) {
			RenderFeature(hoveredTile->GetFeature());
		}
	}
	else if (g_theInventoryLog->m_itemBeingHovered) {
		RenderItemData(g_theInventoryLog->m_itemBeingHovered);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
void TheInfoLog::RenderNPCData(Agent* agent) const {
	if (!agent) {
		return;
	}

	String name = "Name: " + agent->GetName();
	String health = "Health: " + to_string(agent->GetHealth()) + " / " + to_string(agent->GetMaxHealth());
	String atk = "Attack: " + agent->GetDamage().ToString();
	String armor = "Armor: " + to_string(agent->GetArmor());

	if (agent->GetDamage() == RangeInt(0, 0)) {
		atk = "Attack: ?";
	}

	float nameLength = g_theUI->m_font->GetWidthOfString(name, 0.3f);
	float healthLength = g_theUI->m_font->GetWidthOfString(health, 0.3f);
	float atkLength = g_theUI->m_font->GetWidthOfString(atk, 0.3f);
	float armorLength = g_theUI->m_font->GetWidthOfString(armor, 0.3f);

	float nameStart = 1600.f - (10.f + nameLength);
	float healthStart = 1600.f - (10.f + healthLength);
	float atkStart = 1600.f - (10.f + atkLength);
	float armorStart = 1600.f - (10.f + armorLength);

	float start = nameStart;

	if (start > healthStart)
		start = healthStart;
	if (start > atkStart)
		start = atkStart;
	if (start > armorStart)
		start = armorStart;

	g_theUI->DrawTextAtLocation(Vector2(start, 800.f), name, 0.3f, RGBA::WHITE);
	g_theUI->DrawTextAtLocation(Vector2(start, 780.f), health, 0.3f, RGBA::WHITE);
	g_theUI->DrawTextAtLocation(Vector2(start, 760.f), atk, 0.3f, RGBA::WHITE);
	g_theUI->DrawTextAtLocation(Vector2(start, 740.f), armor, 0.3f, RGBA::WHITE);
}
//---------------------------------------------------------------------------------------------------------------------------
void TheInfoLog::RenderItemData(Inventory* inv) const {
	std::vector<Item*>* items = inv->GetItemsInBag();

	if (items->size() == 1) {
		Item* item = items->at(0);

		String name = "Name: " + item->GetName();
		String bonus = item->BonusToString();

		float nameLength = g_theUI->m_font->GetWidthOfString(name, 0.3f);
		float bonusLength = g_theUI->m_font->GetWidthOfString(bonus, 0.3f);

		float nameStart = 1600.f - (10.f + nameLength);
		float bonusStart = 1600.f - (10.f + bonusLength);

		float start = nameStart;

		if (start > bonusStart)
			start = bonusStart;

		g_theUI->DrawTextAtLocation(Vector2(start, 800.f), name, 0.3f, RGBA::WHITE);
		g_theUI->DrawTextAtLocation(Vector2(start, 775.f), bonus, 0.3f, RGBA::WHITE);
	}
	else if (items->size() > 1) {
		Item* item = items->at(0);

		String stack = "A stack of items.";
		String first = "First in stack: ";
		String name = "Name: " + item->GetName();
		String bonus = item->BonusToString();

		float stackLength = g_theUI->m_font->GetWidthOfString(stack, 0.3f);
		float firstLength = g_theUI->m_font->GetWidthOfString(first, 0.3f);
		float nameLength = g_theUI->m_font->GetWidthOfString(name, 0.3f);
		float bonusLength = g_theUI->m_font->GetWidthOfString(bonus, 0.3f);

		float stackStart = 1600.f - (10.f + stackLength);
		float firstStart = 1600.f - (10.f + firstLength);
		float nameStart = 1600.f - (10.f + nameLength);
		float bonusStart = 1600.f - (10.f + bonusLength);

		float start = stackStart;

		if (start > bonusStart)
			start = bonusStart;
		if (start > firstStart)
			start = firstStart;
		if (start > nameStart)
			start = nameStart;
		if (start > bonusStart)
			start = bonusStart;

		g_theUI->DrawTextAtLocation(Vector2(start, 800.f), stack, 0.3f, RGBA::WHITE);
		g_theUI->DrawTextAtLocation(Vector2(start, 800.f), first, 0.3f, RGBA::WHITE);
		g_theUI->DrawTextAtLocation(Vector2(start, 800.f), name, 0.3f, RGBA::WHITE);
		g_theUI->DrawTextAtLocation(Vector2(start, 775.f), bonus, 0.3f, RGBA::WHITE);
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TheInfoLog::RenderItemData(Item* item) const {

	String name = "Name: " + item->GetName();
	String bonus = item->BonusToString();

	float nameLength = g_theUI->m_font->GetWidthOfString(name, 0.3f);
	float bonusLength = g_theUI->m_font->GetWidthOfString(bonus, 0.3f);

	float nameStart = 1600.f - (10.f + nameLength);
	float bonusStart = 1600.f - (10.f + bonusLength);

	float start = nameStart;

	if (start > bonusStart)
		start = bonusStart;

	g_theUI->DrawTextAtLocation(Vector2(start, 800.f), name, 0.3f, RGBA::WHITE);
	g_theUI->DrawTextAtLocation(Vector2(start, 775.f), bonus, 0.3f, RGBA::WHITE);
}
//---------------------------------------------------------------------------------------------------------------------------
void TheInfoLog::RenderFeature(Feature* feature) const {
	String name = "Name: " + feature->GetName();
	String activation = feature->GetInfoText();

	float nameLength = g_theUI->m_font->GetWidthOfString(name, 0.3f);
	float bonusLength = g_theUI->m_font->GetWidthOfString(activation, 0.3f);

	float nameStart = 1600.f - (10.f + nameLength);
	float bonusStart = 1600.f - (10.f + bonusLength);

	float start = nameStart;

	if (start > bonusStart)
		start = bonusStart;

	g_theUI->DrawTextAtLocation(Vector2(start, 800.f), name, 0.3f, RGBA::WHITE);
	g_theUI->DrawTextAtLocation(Vector2(start, 775.f), activation, 0.3f, RGBA::WHITE);
}

//---------------------------------------------------------------------------------------------------------------------------
//ANIM
//---------------------------------------------------------------------------------------------------------------------------
void TheInfoLog::PlaySaveAnimation(bool couldSave) {
	m_saveAge = 5.f;
	m_couldSave = couldSave;
}
//---------------------------------------------------------------------------------------------------------------------------
void TheInfoLog::RenderSaveAnimation() const {
	if (m_couldSave) {
		String drawStr = "Saved Game at Data/SaveGame/SaveGame.xml";
		Vector2 textDrawLoc;
		float widthOfString = g_theUI->m_font->GetWidthOfString(drawStr, 0.5f);
		textDrawLoc.x = ((1600.f / 2.f) - (widthOfString / 2.f));
		textDrawLoc.y = 100.f;
		g_theUI->DrawTextAtLocation(textDrawLoc, drawStr, 0.5f, RGBA(1.f, 1.f, 0.f, m_saveAge));
	}
	else {
		String drawStr = "ERROR: Could not save game! (Hint: Make sure SaveGame.xml is not checked out";
		Vector2 textDrawLoc;
		float widthOfString = g_theUI->m_font->GetWidthOfString(drawStr, 0.5f);
		textDrawLoc.x = ((1600.f / 2.f) - (widthOfString / 2.f));
		textDrawLoc.y = 100.f;
		g_theUI->DrawTextAtLocation(textDrawLoc, drawStr, 0.5f, RGBA(1.f, 1.f, 0.f, m_saveAge));
	}

}
void TheInfoLog::ClearAnimation() {
	m_saveAge = 0.f;
}