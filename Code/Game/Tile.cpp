#include "Game/Tile.hpp"
#include "Game/TileDefinition.hpp"
#include "Game/NPC.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//CHECKS AGAINST TILE
//---------------------------------------------------------------------------------------------------------------------------
bool Tile::IsBlockingPathing() const {
	if (m_containedFeature) {
		return m_containedFeature->IsCurrentlyBlockingPathing();
	}
	else {
		return (m_currTileType == TILE_STONE) ? (true) : (false);
	}
}
bool Tile::IsBlockingLOS() const {
	if (m_containedFeature) {
		return m_containedFeature->IsCurrentlyBlockingLOS();
	}
	else {
		return (m_currTileType == TILE_STONE) ? (true) : (false);
	}
}
bool Tile::IsValid() const {
	if (m_containedFeature) {
		return !m_containedFeature->IsCurrentlyBlockingLOS();
	}
	else {
		return (m_currTileType != TILE_STONE && m_currTileType != TILE_AIR) ? (true) : (false);
	}
}

bool Tile::IsValidForAgent(int movementProperties) const {

	int blockedByProperties = 0;
	if (BitUtils::IsBitSet(movementProperties, BLOCKED_BY_LAVA)) {
		blockedByProperties |= BLOCKED_BY_LAVA;
	}
	if (BitUtils::IsBitSet(movementProperties, BLOCKED_BY_WATER)) {
		blockedByProperties |= BLOCKED_BY_WATER;
	}
	if (BitUtils::IsBitSet(movementProperties, BLOCKED_BY_WALLS)) {
		blockedByProperties |= BLOCKED_BY_WALLS;
	}
	if (BitUtils::IsBitSet(movementProperties, BLOCKED_BY_AGENTS)) {
		blockedByProperties |= BLOCKED_BY_AGENTS;
	}

	int tileProperties = 0;
	if (m_currTileType == TILE_LAVA) {
		tileProperties |= IS_LAVA;
	}
	if (IsBlockingLOS()) {
		tileProperties |= IS_WALL;
	}
	if (m_currTileType == TILE_WATER) {
		tileProperties |= IS_WATER;
	}
	if (m_occupyingAgent) {
		tileProperties |= CONTAINS_AGENT;
	}

	return !(tileProperties & blockedByProperties) && m_currTileType != TILE_AIR && m_occupyingAgent == nullptr;
}

bool Tile::IsDangerous() const {
	return (m_currTileType == TILE_LAVA);
}

char Tile::GetGlyph() const {
	return (!m_containedFeature) ? (TileDefinition::GetTileGlyph(m_currTileType)) : (m_containedFeature->GetGlyph());
}
RGBA Tile::GetColor() const {
	return (!m_containedFeature) ? (TileDefinition::GetColor(m_currTileType)) : (m_containedFeature->GetColor());
}
RGBA Tile::GetBackgroundColor() const {
	return (!m_containedFeature) ? (TileDefinition::GetBackgroundColor(m_currTileType)) : (m_containedFeature->GetBackgroundColor());
}

//---------------------------------------------------------------------------------------------------------------------------
//STATIC FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
STATIC eTileType Tile::ParseTileTypeFromString(const String& strToParse) {
	String str = strToParse;
	StringUtils::ToLowerCase(str);

	if (str == "stone") {
		return TILE_STONE;
	}
	else if (str == "water") {
		return TILE_WATER;
	}
	else if (str == "air") {
		return TILE_AIR;
	}
	else if (str == "grass") {
		return TILE_GRASS;
	}
	else if (str == "lava") {
		return TILE_LAVA;
	}

	return TILE_GRASS;
}

void Tile::ClearInventory() {
	if (m_inventory) {
		delete m_inventory;
		m_inventory = nullptr;
	}
}