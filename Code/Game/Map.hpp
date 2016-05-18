#pragma once

#define UNREFERENCED_PARAMETER(x) (x)

#include <vector>
#include "Engine/General/Utils/ErrorWarningAssert.hpp"
#include "Game/Tile.hpp"
#include "Game/DynamicTileProperty.hpp"
#include "Game/NPCFactory.hpp"

class Map {
public:
	//STRUCTORS
	INLINE Map();
	Map(IntVector2 size);
	Map(IntVector2 size, eTileType startingTileType);

	//UPDATE
	void Update(float deltaSeconds);

	void RenderGlyphAtLocation(char glyph, const IntVector2& loc, const RGBA& col);

	//UTILITY FUNCS
	INLINE IntVector2 GetSize() const;
	INLINE Tile* GetTileAtIndex(unsigned int idx);
	INLINE bool CheckIfTileIsOfTypeAtLoc(const IntVector2& loc, eTileType type);
	INLINE bool LocIsInMapBounds(const IntVector2& loc);
	int GetNumberOfTilesSurroundingOfType(const IntVector2& loc, eTileType type, int radius);
	Tile* GetTileAtLocation(const IntVector2& loc);
	bool AreNeighborsAllSolid(const IntVector2& loc);

	int GetManhattanDistance(const IntVector2& start, const IntVector2& end);

	IntVector2 FindRandomValidLocation();
	IntVector2 FindRandomValidLocationToSpawn();
	std::vector<IntVector2> GetValidLocsToMoveToAroundLoc(const IntVector2& loc, int movementProperties);

	IntVector2 GetLocationWithOpeningOnEitherSide();

	static bool IsLocAdjacentToLoc(const IntVector2& loc1, const IntVector2& loc2);

	//SAVING LOADING
	XMLNode GetMapLegend();
	String GetMapDataAsText();
	String GetVisibilityAsText();
	XMLNode SaveToXML();

public:
	std::vector<Tile> m_tiles;
	IntVector2 m_size;

	std::vector<DynamicTileProperty> m_dynamicallyRenderedTiles;
};

////////////////////////////////////////////////////
//------------------------------------------
//INLINE
inline Map::Map()
	: m_tiles()
	, m_size()
{ }


//---------------------------------------------------------------------------------------------------------------------------
//UTILITY FUNCS
//---------------------------------------------------------------------------------------------------------------------------
inline Tile* Map::GetTileAtIndex(unsigned int idx) {
	return &m_tiles[idx];
}
inline IntVector2 Map::GetSize() const {
	return m_size;
}

//---------------------------------------------------------------------------------------------------------------------------
inline bool Map::CheckIfTileIsOfTypeAtLoc(const IntVector2& loc, eTileType type) {
	Tile* tile = GetTileAtLocation(loc);
	return (nullptr != tile && tile->GetCurrentTileType() == type);
}

//---------------------------------------------------------------------------------------------------------------------------
inline bool Map::LocIsInMapBounds(const IntVector2& loc) {
	return (loc.x < m_size.x && loc.y < m_size.y && loc.x >= 0 && loc.y >= 0);
}