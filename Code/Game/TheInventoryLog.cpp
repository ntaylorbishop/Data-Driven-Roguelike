#include "Game/TheInventoryLog.hpp"
#include "Game/TheUI.hpp"
#include "Game/TheWorld.hpp"
#include "Game/Item.hpp"
#include "Game/Player.hpp"
#include "Game/Equipment.hpp"

TheInventoryLog* g_theInventoryLog = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
TheInventoryLog::TheInventoryLog() 
	: m_bg(nullptr)
	, m_itemBeingHovered(nullptr)
{
	m_bg = Texture::CreateOrGetTexture("Data/Images/INV_BG.png");
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void TheInventoryLog::Update(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

	m_itemBeingHovered = nullptr;
	m_itemIsInInventory = false;

	GetItemOfMouseOver();
	HandleInput();
}

void TheInventoryLog::HandleInput() {
	if (m_itemBeingHovered) {
		if (g_theInputSystem->GetMouseBtnDown(MOUSE_LEFT)) {
			if (m_itemIsInInventory) {
				switch (m_itemBeingHovered->GetType()) {
				case ITEM_WEAPON:
					g_theWorld->m_thePlayer->EquipItem(m_itemBeingHovered);
					break;
				case ITEM_ARMOR:
					g_theWorld->m_thePlayer->EquipItem(m_itemBeingHovered);
					break;
				case ITEM_POTION:
					g_theWorld->m_thePlayer->UseItem(m_itemBeingHovered);
					break;
				}
			}
			else {
				switch (m_itemBeingHovered->GetType()) {
				case ITEM_WEAPON:
					g_theWorld->m_thePlayer->UnequipItem(m_itemBeingHovered);
					break;
				case ITEM_ARMOR:
					g_theWorld->m_thePlayer->UnequipItem(m_itemBeingHovered);
					break;
				case ITEM_POTION:
					DebuggerPrintf("Clicked on a potion in your equipment. This shouldn't have happened.\n");
					break;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheInventoryLog::Render() const {
	if (g_theWorld->m_thePlayer) {
		//Render background
		g_theRenderer->DrawTexturedAABB2(m_bg, RGBA(1.f, 0.f, 1.f, 1.f), AABB2(Vector2(1600.f - INV_WIDTH, 0.f), Vector2(1600.f, 700.f)));

		//Inventory
		g_theUI->DrawTextAtLocation(Vector2(INV_START_X + 50.f, 560.f), "INVENTORY", 0.4f, RGBA::WHITE);
		g_theRenderer->DrawAABB2(AABB2(Vector2(INV_START_X + 10.f, 350.f), Vector2(INV_START_X + 190.f, 350.f + 5.f * TILE_SIZE.y)), SLOT_COLOR_UNHIGHLIGHTED, PRIMITIVE_QUADS);
		RenderInventory();

		//Equipment
		g_theUI->DrawTextAtLocation(Vector2(INV_START_X + 50.f, 120.f), "EQUIPMENT", 0.4f, RGBA::WHITE);
		g_theRenderer->DrawAABB2(AABB2(Vector2(INV_START_X + 60.f, 60.f), Vector2(INV_START_X + 60.f + 2.f * TILE_SIZE.x, 60.f + TILE_SIZE.y)), SLOT_COLOR_UNHIGHLIGHTED, PRIMITIVE_QUADS);
		RenderEquipment();

		RenderHighlight();
	}
}

void TheInventoryLog::RenderInventory() const {
	//RENDER BACKGROUND
	Vector2 currLoc = Vector2(INV_START_X + 10.f, 350.f);

	int itemIdx = 0;
	for (unsigned int x = 0; x < 5; x++) {
		Vector2 currLocX = currLoc;

		for (unsigned int y = 0; y < 4; y++) {
			g_theRenderer->DrawSquare(AABB2(currLocX, currLocX + TILE_SIZE), RGBA::BLACK, 2.f);

			Item* itemToRender = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(itemIdx);
			if (itemToRender) {
				String glyph;
				glyph.push_back(itemToRender->GetGlyph());
				g_theUI->DrawTextAtLocation(currLocX + Vector2(9.f, 7.f), glyph, 0.4f, itemToRender->GetColor());
			}

			currLocX = currLocX + Vector2(TILE_SIZE.x, 0.f);
			itemIdx++;
		}

		currLoc = currLoc + Vector2(0.f, TILE_SIZE.y);
	}
}
void TheInventoryLog::RenderEquipment() const {

	//WEAPON SLOT
	Vector2 weaponSlotLoc = Vector2(INV_START_X + 60.f, 60.f);
	g_theRenderer->DrawSquare(AABB2(weaponSlotLoc, weaponSlotLoc + TILE_SIZE), RGBA::BLACK, 2.f);
	g_theWorld->m_squirrelFont->DrawText2D(weaponSlotLoc + Vector2(5.f, 5.f), "W", 30.f, RGBA(0.f, 0.f, 0.f, 0.4f));

	Vector2 start = Vector2(INV_START_X + 60.f, 60.f);
	Item* itemToRender = g_theWorld->m_thePlayer->GetEquipment()->GetWeaponInWeaponSlot();
	if (itemToRender) {
		String glyph;
		glyph.push_back(itemToRender->GetGlyph());
		g_theUI->DrawTextAtLocation(start + Vector2(10.f, 8.f), glyph, 0.4f, itemToRender->GetColor());
	}

	//ARMOR SLOT
	Vector2 armorSlotLoc = weaponSlotLoc + Vector2(TILE_SIZE.x, 0.f);
	g_theRenderer->DrawSquare(AABB2(armorSlotLoc, armorSlotLoc + TILE_SIZE), RGBA::BLACK, 2.f);
	g_theWorld->m_squirrelFont->DrawText2D(armorSlotLoc + Vector2(5.f, 5.f), "A", 30.f, RGBA(0.f, 0.f, 0.f, 0.4f));

	Vector2 armorStart = start + Vector2(1.f * TILE_SIZE.x, 0.f * TILE_SIZE.y);
	itemToRender = g_theWorld->m_thePlayer->GetEquipment()->GetArmorInArmorSlot();
	if (itemToRender) {
		String glyph;
		glyph.push_back(itemToRender->GetGlyph());
		g_theUI->DrawTextAtLocation(armorStart + Vector2(10.f, 8.f), glyph, 0.4f, itemToRender->GetColor());
	}
}

void TheInventoryLog::RenderHighlight() const {
	if (m_highlightSlotQuad != AABB2(Vector2(-1, -1), Vector2(-1, -1))) {
		g_theRenderer->DrawAABB2(m_highlightSlotQuad, HIGHLIGHT_COLOR, PRIMITIVE_QUADS);
	}
}
//---------------------------------------------------------------------------------------------------------------------------
//ITERACTIVITY
//---------------------------------------------------------------------------------------------------------------------------
void TheInventoryLog::GetItemOfMouseOver() {
	Vector2 relMousePos = g_theInputSystem->GetRelativeCursorPosition().ToVector2();

	if (IsMouseWithinInventory()) {
		Vector2 start = Vector2(INV_START_X + 10.f, 350.f);
		AABB2 range = AABB2(Vector2(INV_START_X + 10.f, 350.f), Vector2(INV_START_X + 190.f, 350.f + 5.f * TILE_SIZE.y));

		AABB2 invSlot1  = AABB2(start + Vector2(0.f * TILE_SIZE.x, 0.f * TILE_SIZE.y), start + Vector2(1.f * TILE_SIZE.x, 1.f * TILE_SIZE.y));
		AABB2 invSlot2  = AABB2(start + Vector2(1.f * TILE_SIZE.x, 0.f * TILE_SIZE.y), start + Vector2(2.f * TILE_SIZE.x, 1.f * TILE_SIZE.y));
		AABB2 invSlot3  = AABB2(start + Vector2(2.f * TILE_SIZE.x, 0.f * TILE_SIZE.y), start + Vector2(3.f * TILE_SIZE.x, 1.f * TILE_SIZE.y));
		AABB2 invSlot4  = AABB2(start + Vector2(3.f * TILE_SIZE.x, 0.f * TILE_SIZE.y), start + Vector2(4.f * TILE_SIZE.x, 1.f * TILE_SIZE.y));
		AABB2 invSlot5  = AABB2(start + Vector2(0.f * TILE_SIZE.x, 1.f * TILE_SIZE.y), start + Vector2(1.f * TILE_SIZE.x, 2.f * TILE_SIZE.y));
		AABB2 invSlot6  = AABB2(start + Vector2(1.f * TILE_SIZE.x, 1.f * TILE_SIZE.y), start + Vector2(2.f * TILE_SIZE.x, 2.f * TILE_SIZE.y));
		AABB2 invSlot7  = AABB2(start + Vector2(2.f * TILE_SIZE.x, 1.f * TILE_SIZE.y), start + Vector2(3.f * TILE_SIZE.x, 2.f * TILE_SIZE.y));
		AABB2 invSlot8  = AABB2(start + Vector2(3.f * TILE_SIZE.x, 1.f * TILE_SIZE.y), start + Vector2(4.f * TILE_SIZE.x, 2.f * TILE_SIZE.y));
		AABB2 invSlot9  = AABB2(start + Vector2(0.f * TILE_SIZE.x, 2.f * TILE_SIZE.y), start + Vector2(1.f * TILE_SIZE.x, 3.f * TILE_SIZE.y));
		AABB2 invSlot10 = AABB2(start + Vector2(1.f * TILE_SIZE.x, 2.f * TILE_SIZE.y), start + Vector2(2.f * TILE_SIZE.x, 3.f * TILE_SIZE.y));
		AABB2 invSlot11 = AABB2(start + Vector2(2.f * TILE_SIZE.x, 2.f * TILE_SIZE.y), start + Vector2(3.f * TILE_SIZE.x, 3.f * TILE_SIZE.y));
		AABB2 invSlot12 = AABB2(start + Vector2(3.f * TILE_SIZE.x, 2.f * TILE_SIZE.y), start + Vector2(4.f * TILE_SIZE.x, 3.f * TILE_SIZE.y));
		AABB2 invSlot13 = AABB2(start + Vector2(0.f * TILE_SIZE.x, 3.f * TILE_SIZE.y), start + Vector2(1.f * TILE_SIZE.x, 4.f * TILE_SIZE.y));
		AABB2 invSlot14 = AABB2(start + Vector2(1.f * TILE_SIZE.x, 3.f * TILE_SIZE.y), start + Vector2(2.f * TILE_SIZE.x, 4.f * TILE_SIZE.y));
		AABB2 invSlot15 = AABB2(start + Vector2(2.f * TILE_SIZE.x, 3.f * TILE_SIZE.y), start + Vector2(3.f * TILE_SIZE.x, 4.f * TILE_SIZE.y));
		AABB2 invSlot16 = AABB2(start + Vector2(3.f * TILE_SIZE.x, 3.f * TILE_SIZE.y), start + Vector2(4.f * TILE_SIZE.x, 4.f * TILE_SIZE.y));
		AABB2 invSlot17 = AABB2(start + Vector2(0.f * TILE_SIZE.x, 4.f * TILE_SIZE.y), start + Vector2(1.f * TILE_SIZE.x, 5.f * TILE_SIZE.y));
		AABB2 invSlot18 = AABB2(start + Vector2(1.f * TILE_SIZE.x, 4.f * TILE_SIZE.y), start + Vector2(2.f * TILE_SIZE.x, 5.f * TILE_SIZE.y));
		AABB2 invSlot19 = AABB2(start + Vector2(2.f * TILE_SIZE.x, 4.f * TILE_SIZE.y), start + Vector2(3.f * TILE_SIZE.x, 5.f * TILE_SIZE.y));
		AABB2 invSlot20 = AABB2(start + Vector2(3.f * TILE_SIZE.x, 4.f * TILE_SIZE.y), start + Vector2(4.f * TILE_SIZE.x, 5.f * TILE_SIZE.y));

		if (g_theInputSystem->IsMouseWithinQuad(invSlot1)) {
			m_highlightSlotQuad = invSlot1;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(0);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot2)) {
			m_highlightSlotQuad = invSlot2;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(1);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot3)) {
			m_highlightSlotQuad = invSlot3;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(2);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot4)) {
			m_highlightSlotQuad = invSlot4;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(3);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot5)) {
			m_highlightSlotQuad = invSlot5;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(4);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot6)) {
			m_highlightSlotQuad = invSlot6;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(5);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot7)) {
			m_highlightSlotQuad = invSlot7;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(6);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot8)) {
			m_highlightSlotQuad = invSlot8;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(7);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot9)) {
			m_highlightSlotQuad = invSlot9;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(8);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot10)) {
			m_highlightSlotQuad = invSlot10;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(9);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot11)) {
			m_highlightSlotQuad = invSlot11;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(10);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot12)) {
			m_highlightSlotQuad = invSlot12;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(11);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot13)) {
			m_highlightSlotQuad = invSlot13;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(12);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot14)) {
			m_highlightSlotQuad = invSlot14;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(13);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot15)) {
			m_highlightSlotQuad = invSlot15;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(14);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot16)) {
			m_highlightSlotQuad = invSlot16;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(15);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot17)) {
			m_highlightSlotQuad = invSlot17;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(16);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot18)) {
			m_highlightSlotQuad = invSlot18;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(17);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot19)) {
			m_highlightSlotQuad = invSlot19;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(18);
		}
		if (g_theInputSystem->IsMouseWithinQuad(invSlot20)) {
			m_highlightSlotQuad = invSlot20;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetInventory()->GetItemInSlot(19);
		}

		m_itemIsInInventory = true;
	}
	else if (IsMouseWithinEquipment()) {

		Vector2 start = Vector2(INV_START_X + 60.f, 60.f);

		AABB2 eSlot1 = AABB2(start + Vector2(0.f * TILE_SIZE.x, 0.f * TILE_SIZE.y), start + Vector2(1.f * TILE_SIZE.x, 1.f * TILE_SIZE.y));
		AABB2 eSlot2 = AABB2(start + Vector2(1.f * TILE_SIZE.x, 0.f * TILE_SIZE.y), start + Vector2(2.f * TILE_SIZE.x, 1.f * TILE_SIZE.y));
		
		if (g_theInputSystem->IsMouseWithinQuad(eSlot1)) {
			m_highlightSlotQuad = eSlot1;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetEquipment()->GetWeaponInWeaponSlot();
		}
		if (g_theInputSystem->IsMouseWithinQuad(eSlot2)) {
			m_highlightSlotQuad = eSlot2;
			m_itemBeingHovered = g_theWorld->m_thePlayer->GetEquipment()->GetArmorInArmorSlot();
		}

		m_itemIsInInventory = false;
	}
	else {
		m_highlightSlotQuad = AABB2(Vector2(-1, -1), Vector2(-1, -1));
		m_itemIsInInventory = nullptr;
	}
}

////////////////////////////////////////////////////
//------------------------------------------
//INVENTORY
bool TheInventoryLog::IsMouseWithinInventory() const {

	Vector2 relMousePos = g_theInputSystem->GetRelativeCursorPosition().ToVector2();
	AABB2 range = AABB2(Vector2(INV_START_X + 10.f, 350.f), Vector2(INV_START_X + 190.f, 350.f + 5.f * TILE_SIZE.y));

	return g_theInputSystem->IsMouseWithinQuad(range);
}

////////////////////////////////////////////////////
//------------------------------------------
//EQUIPMENT
bool TheInventoryLog::IsMouseWithinEquipment() const {

	Vector2 relMousePos = g_theInputSystem->GetRelativeCursorPosition().ToVector2();
	AABB2 range = AABB2(Vector2(INV_START_X + 60.f, 60.f), Vector2(INV_START_X + 60.f + 2.f * TILE_SIZE.x, 60.f + TILE_SIZE.y));

	return g_theInputSystem->IsMouseWithinQuad(range);
}