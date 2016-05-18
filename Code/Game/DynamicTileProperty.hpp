#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/MathUtils.hpp"

class Tile;

class DynamicTileProperty {
public:
	//STRUCTORS
	DynamicTileProperty();
	DynamicTileProperty(Tile* tile);

	//UPDATE
	void Update(float deltaSeconds);
	void UpdateWaterTile();
	void UpdateLavaTile();

private:
	Tile* m_tileToUpdate;
	float m_age;



	//FOR WATER
	float m_whenToFlip;
	std::vector<RGBA> m_possibleWaterColors;
};