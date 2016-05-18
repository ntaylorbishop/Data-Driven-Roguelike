#include "Game/DynamicTileProperty.hpp"
#include "Game/Tile.hpp"
#include <math.h>

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
DynamicTileProperty::DynamicTileProperty()
	: m_tileToUpdate(nullptr)
	, m_age(0.f)
	, m_whenToFlip(0.f)
{ }
DynamicTileProperty::DynamicTileProperty(Tile* tile)
	: m_tileToUpdate(tile)
	, m_age(0.f)
	, m_whenToFlip(0.f)
{
	m_whenToFlip = Randf(1.5f, 2.2f);
	m_possibleWaterColors = {
		RGBA(0x76, 0x01, 0xff, 0xAA),
		RGBA(0x00, 0x36, 0xff, 0xAA),
		RGBA(0x48, 0x4c, 0xea, 0xAA),
		RGBA(0x00, 0x78, 0xff, 0xAA)
	};

	if (tile->GetCurrentTileType() == TILE_LAVA) {
		m_age = Randf(0.f, 10.f);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void DynamicTileProperty::Update(float deltaSeconds) {
	m_age += deltaSeconds;

	if (m_tileToUpdate) {
		if (m_tileToUpdate->GetCurrentTileType() == TILE_WATER) {
			UpdateWaterTile();
		}
		else if (m_tileToUpdate->GetCurrentTileType() == TILE_LAVA) {
			UpdateLavaTile();
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void DynamicTileProperty::UpdateWaterTile() {
	if (m_age >= m_whenToFlip) {
		m_age = 0.f;

		int whichColor = RandInt(0, m_possibleWaterColors.size() - 1);

		m_tileToUpdate->SetDynamicColor(m_possibleWaterColors[whichColor]);
	}
}
void DynamicTileProperty::UpdateLavaTile() {
	if (m_age >= m_whenToFlip) {

		float flux = 0.5f * sin(m_age) + 0.5f;
		m_tileToUpdate->SetDynamicColor(RGBA(flux, 0.f, 0.f, 1.f));
	}
}