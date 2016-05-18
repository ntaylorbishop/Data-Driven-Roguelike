#include "Game/TileDefinition.hpp"

STATIC TileDefinition* TileDefinition::m_tileDefinitions[NUM_TILE_TYPES];

//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
STATIC void TileDefinition::InitializeTileDefinitions() {
	m_tileDefinitions[TILE_AIR] = new TileDefinition(' ', RGBA::BLACK, RGBA::BLACK);
	m_tileDefinitions[TILE_STONE] = new TileDefinition('%', RGBA::ORANGE, RGBA::BLACK);
	m_tileDefinitions[TILE_GRASS] = new TileDefinition('.', RGBA(0.5f, 0.5f, 0.5f, 0.5f), RGBA(0x00, 0x01, 0x10, 0xFF));
	m_tileDefinitions[TILE_WATER] = new TileDefinition('$', RGBA::BLUE,  RGBA::BLACK);
	m_tileDefinitions[TILE_LAVA] = new TileDefinition('x', RGBA::RED, RGBA::RED);
}
STATIC void TileDefinition::FlushTileDefinitions() {
	delete m_tileDefinitions[TILE_GRASS];
	delete m_tileDefinitions[TILE_WATER];
	delete m_tileDefinitions[TILE_STONE];
}