#pragma once

#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Game/GameCommon.hpp"

class Item;
class TheInventoryLog;
extern TheInventoryLog* g_theInventoryLog;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTS
//---------------------------------------------------------------------------------------------------------------------------
enum eSlotType {
	SLOT_INVENTORY = 0,
	SLOT_EQUIPMENT,
	SLOT_NUM_SLOTS
};
//---------------------------------------------------------------------------------------------------------------------------
//CONSTANTS
//---------------------------------------------------------------------------------------------------------------------------
const float INV_WIDTH = 1600.f - SCREEN_SIZE.x;
const float INV_START_X = 1600.f - INV_WIDTH;
const Vector2 TILE_SIZE = Vector2(45.f, 40.f);

const RGBA SLOT_COLOR_UNHIGHLIGHTED = RGBA(0.5f, 0.5f, 0.5f, 0.5f);
const RGBA SLOT_COLOR_HIGHLIGHTED = RGBA(0.5f, 0.5f, 0.5f, 1.f);

//---------------------------------------------------------------------------------------------------------------------------
//INVENTORY LOG
//---------------------------------------------------------------------------------------------------------------------------
class TheInventoryLog {
public:
	//STRUCTORS
	TheInventoryLog();

	//UPDATE
	void Update(float deltaSeconds);

	void HandleInput();

	//RENDER
	void Render() const;

	void RenderInventory() const;
	void RenderEquipment() const;

	void RenderHighlight() const;

	//INTERACTIVITY
	void GetItemOfMouseOver();

	bool IsMouseWithinInventory() const;
	bool IsMouseWithinEquipment() const;

	Item* m_itemBeingHovered;
	bool m_itemIsInInventory;
private:
	Texture* m_bg;
	AABB2 m_highlightSlotQuad;
};