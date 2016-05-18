#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Game/Agent.hpp"
#include "Game/Inventory.hpp"
#include "Game/Feature.hpp"

enum eMovementProperties;

//---------------------------------------------------------------------------------------------------------------------------
//ENUM TILETYPE
//---------------------------------------------------------------------------------------------------------------------------
enum eTileType {
	TILE_AIR = 0,
	TILE_STONE,
	TILE_GRASS,
	TILE_WATER,
	TILE_LAVA,
	NUM_TILE_TYPES
};

//---------------------------------------------------------------------------------------------------------------------------
//CLASS TILE
//---------------------------------------------------------------------------------------------------------------------------
class Tile {
public:
	//STRUCTORS
	INLINE Tile();
	INLINE Tile(const IntVector2& location, eTileType type);

	//GETTERS SETTERS
	eTileType GetCurrentTileType() const { return m_currTileType; }
	void SetCurrentTileType(eTileType type) { m_currTileType = type; }

	IntVector2 GetLocation() const { return m_location; }
	void SetLocation(const IntVector2& location) { m_location = location; }

	eTileType GetFutureTileType() const { return m_futureTileType; }
	void SetFutureTileType(eTileType type) { m_futureTileType = type; }

	RGBA GetDynamicColor() const { return m_dynamicColor; }
	void SetDynamicColor(const RGBA& col) { m_dynamicColor = col; }

	bool IsDynamicallyRendered() const { return (m_currTileType == TILE_WATER || m_currTileType == TILE_LAVA); }

	//FOG OF WAR
	bool IsVisible() { return m_visible; }
	void SetVisibility(bool visibility) { m_visible = visibility; }
	bool IsExplored() { return m_explored; }
	void Explore() { m_explored = true; }

	//AGENT
	Agent* GetOccupyingAgent() const { return m_occupyingAgent; }
	void SetOccupyingAgent(Agent* agent) { m_occupyingAgent = agent; }
	Inventory* GetInventory() const { return m_inventory; }
	void SetInventory(Inventory* inv) { m_inventory = inv; }
	void ClearInventory();

	Feature* GetFeature() const { return m_containedFeature; }
	void SetFeature(Feature* feature) { m_containedFeature = feature; }

	//CHECKS AGAINST TILE
	bool IsBlockingPathing() const;
	bool IsBlockingLOS() const;
	bool IsValid() const;
	bool IsDangerous() const;
	bool IsValidForAgent(int movementProperties) const;

	char GetGlyph() const;
	RGBA GetColor() const;
	RGBA GetBackgroundColor() const;


	//STATIC FUNCTIONS
	static eTileType ParseTileTypeFromString(const String& strToParse);

	bool m_isBeingHoveredOn = false;
	AABB2 m_renderingBox;

private:
	IntVector2 m_location;

	eTileType m_currTileType;
	eTileType m_futureTileType;

	Agent* m_occupyingAgent;
	Inventory* m_inventory;
	Feature* m_containedFeature;

	bool m_visible = true;
	bool m_explored = false;

	RGBA m_dynamicColor;
};


////////////////////////////////////////////////////
//------------------------------------------
//INLINE

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
inline Tile::Tile()
	: m_location(IntVector2(0, 0))
	, m_currTileType(TILE_GRASS)
	, m_futureTileType(TILE_GRASS)
	, m_occupyingAgent(nullptr)
	, m_inventory(nullptr)
	, m_containedFeature(nullptr)
{ }
inline Tile::Tile(const IntVector2& location, eTileType type)
	: m_location(location)
	, m_currTileType(type)
	, m_futureTileType(TILE_GRASS)
	, m_occupyingAgent(nullptr)
	, m_inventory(nullptr)
	, m_containedFeature(nullptr)
{ }