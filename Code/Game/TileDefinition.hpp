#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Tile.hpp"

class TileDefinition {
public:
	static void TileDefinition::InitializeTileDefinitions();
	static void TileDefinition::FlushTileDefinitions();

	static inline TileDefinition* TileDefinition::GetTileDefinitionByType(eTileType type);

	//ACCESSORS MUTATORS
	static inline char GetTileGlyph(eTileType type);
	static inline RGBA GetColor(eTileType type);
	static inline RGBA GetBackgroundColor(eTileType type);

	static inline char GetTileGlyph(Tile* tile);
	static inline RGBA GetColor(Tile* tile);
	static inline RGBA GetBackgroundColor(Tile* tile);

private:
	//STRUCTORS
	TileDefinition();
	TileDefinition(char tileGlyph, const RGBA& color, const RGBA& backgroundColor);

	INLINE char GetTileGlyph();
	INLINE RGBA GetColor();
	INLINE RGBA GetBackgroundColor();

	INLINE char m_tileGlyph;
	INLINE RGBA m_color;
	INLINE RGBA m_backgroundColor;

	static TileDefinition* m_tileDefinitions[NUM_TILE_TYPES];
};


////////////////////////////////////////////////////
//------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
STATIC TileDefinition* TileDefinition::GetTileDefinitionByType(eTileType type) {
	return m_tileDefinitions[type];
}

//---------------------------------------------------------------------------------------------------------------------------
//ACCESSORS MUTATORS
//---------------------------------------------------------------------------------------------------------------------------
STATIC inline char TileDefinition::GetTileGlyph(eTileType type) {
	return m_tileDefinitions[type]->GetTileGlyph();
}
STATIC inline RGBA TileDefinition::GetColor(eTileType type) {
	return m_tileDefinitions[type]->GetColor();
}
STATIC inline RGBA TileDefinition::GetBackgroundColor(eTileType type) {
	return m_tileDefinitions[type]->GetBackgroundColor();
}

STATIC inline char TileDefinition::GetTileGlyph(Tile* tile) {
	ASSERT_OR_DIE(nullptr != tile, "ERROR: NULL TILE PASSED IN TO TILE DEFINITION");
	return m_tileDefinitions[tile->GetCurrentTileType()]->GetTileGlyph();
}
STATIC inline RGBA TileDefinition::GetColor(Tile* tile) {
	ASSERT_OR_DIE(nullptr != tile, "ERROR: NULL TILE PASSED IN TO TILE DEFINITION");
	return m_tileDefinitions[tile->GetCurrentTileType()]->GetColor();
}
STATIC inline RGBA TileDefinition::GetBackgroundColor(Tile* tile) {
	ASSERT_OR_DIE(nullptr != tile, "ERROR: NULL TILE PASSED IN TO TILE DEFINITION");
	return m_tileDefinitions[tile->GetCurrentTileType()]->GetBackgroundColor();
}

//---------------------------------------------------------------------------------------------------------------------------
//MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
inline TileDefinition::TileDefinition() { }
inline TileDefinition::TileDefinition(char tileGlyph, const RGBA& color, const RGBA& backgroundColor)
	: m_tileGlyph(tileGlyph)
	, m_color(color)
	, m_backgroundColor(backgroundColor)
{  }

inline char TileDefinition::GetTileGlyph() {
	return m_tileGlyph;
}
inline RGBA TileDefinition::GetColor() {
	return m_color;
}
inline RGBA TileDefinition::GetBackgroundColor() {
	return m_backgroundColor;
}