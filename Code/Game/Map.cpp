#include "Game/Map.hpp"
#include "Game/TheUI.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Map::Map(IntVector2 size) {
	m_tiles.reserve(size.x * size.y);
	m_size = size;

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			m_tiles.push_back(Tile(IntVector2(x, y), TILE_GRASS));
		}
	}
}
Map::Map(IntVector2 size, eTileType startingTileType) {
	m_tiles.reserve(size.x * size.y);
	m_size = size;

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			m_tiles.push_back(Tile(IntVector2(x, y), startingTileType));
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void Map::Update(float deltaSeconds) {
	for (unsigned int i = 0; i < m_dynamicallyRenderedTiles.size(); i++) {
		m_dynamicallyRenderedTiles[i].Update(deltaSeconds);
	}
}

void Map::RenderGlyphAtLocation(char glyph, const IntVector2& loc, const RGBA& col) {

	IntVector2 numTilesOnScreen = GetSize();
	Vector2 cellSize = Vector2(SCREEN_SIZE.x / (float)numTilesOnScreen.x, SCREEN_SIZE.y / (float)numTilesOnScreen.y);

	Vector2 positionToDrawFrom = loc.ToVector2() * cellSize;
	Vector2 positionToDrawTo = positionToDrawFrom + cellSize;

	String glyphStr = "";
	glyphStr.push_back(glyph);
	float lineHeight = abs((positionToDrawFrom.y - positionToDrawTo.y) * 0.75f);

	g_theUI->m_squirrelFont->DrawText2D(positionToDrawFrom, glyphStr, lineHeight, col);
}

//---------------------------------------------------------------------------------------------------------------------------
//UTILITY FUNCS
//---------------------------------------------------------------------------------------------------------------------------
int Map::GetNumberOfTilesSurroundingOfType(const IntVector2& loc, eTileType type, int radius) {
	int numTiles = 0;

	//North
	for (int i = 1; i <= radius; i++) {
		//NORTH
		if (CheckIfTileIsOfTypeAtLoc(loc + IntVector2(0, i), type))
			numTiles++;
		//NORTHEAST
		if (CheckIfTileIsOfTypeAtLoc(loc + IntVector2(i, i), type))
			numTiles++;
		//EAST
		if (CheckIfTileIsOfTypeAtLoc(loc + IntVector2(i, 0), type))
			numTiles++;
		//SOUTHEAST
		if (CheckIfTileIsOfTypeAtLoc(loc + IntVector2(i, -i), type))
			numTiles++;
		//SOUTH
		if (CheckIfTileIsOfTypeAtLoc(loc + IntVector2(0, -i), type))
			numTiles++;
		//SOUTHWEST
		if (CheckIfTileIsOfTypeAtLoc(loc + IntVector2(-i, -i), type))
			numTiles++;
		//WEST
		if (CheckIfTileIsOfTypeAtLoc(loc + IntVector2(-i, 0), type))
			numTiles++;
		//NORTHWEST
		if (CheckIfTileIsOfTypeAtLoc(loc + IntVector2(-i, i), type))
			numTiles++;
	}

	return numTiles;
}

//---------------------------------------------------------------------------------------------------------------------------
Tile* Map::GetTileAtLocation(const IntVector2& loc) {
	unsigned int idx = (loc.x * m_size.y) + loc.y;
	return (idx >= m_tiles.size()) ? nullptr : &m_tiles[idx];
}

//---------------------------------------------------------------------------------------------------------------------------
bool Map::AreNeighborsAllSolid(const IntVector2& loc) {
	UNREFERENCED_PARAMETER(loc);

	int numTilesSolid = 0;

	std::vector<Tile*> surroundingTiles;

	surroundingTiles.push_back(GetTileAtLocation(loc + IntVector2(-1,  1)));
	surroundingTiles.push_back(GetTileAtLocation(loc + IntVector2( 0,  1)));
	surroundingTiles.push_back(GetTileAtLocation(loc + IntVector2( 1,  1)));
	surroundingTiles.push_back(GetTileAtLocation(loc + IntVector2( 1,  0)));
	surroundingTiles.push_back(GetTileAtLocation(loc + IntVector2( 1, -1)));
	surroundingTiles.push_back(GetTileAtLocation(loc + IntVector2( 0, -1)));
	surroundingTiles.push_back(GetTileAtLocation(loc + IntVector2(-1, -1)));
	surroundingTiles.push_back(GetTileAtLocation(loc + IntVector2(-1,  0)));

	for (unsigned int i = 0; i < surroundingTiles.size(); i++) {
		if (nullptr != surroundingTiles[i] && surroundingTiles[i]->IsBlockingPathing()) {
			numTilesSolid++;
		}
	}

	if (numTilesSolid == 8) {
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
IntVector2 Map::FindRandomValidLocation() {
	IntVector2 position;
	bool foundALocation = false;
	int indicesToConsider = m_tiles.size();

	int numTimesRan = 0;

	while (!foundALocation) {
		int ind = RandInt(0, indicesToConsider - 1);

		Tile* currTile = GetTileAtIndex(ind);

		if (nullptr != currTile && currTile->IsValid()) {
			position = currTile->GetLocation();
			foundALocation = true;
		}

		
		if (numTimesRan > 10000) {
			DebuggerPrintf("ERROR: Find Random Valid Location running too long.");
			return IntVector2(0, 0);
		}
		numTimesRan++;
	}

	return position;
}
IntVector2 Map::FindRandomValidLocationToSpawn() {
	IntVector2 position;
	bool foundALocation = false;
	int indicesToConsider = m_tiles.size();

	int numTimesRan = 0;

	while (!foundALocation) {
		int ind = RandInt(0, indicesToConsider - 1);

		Tile* currTile = GetTileAtIndex(ind);

		if (nullptr != currTile && currTile->IsValid() && currTile->GetCurrentTileType() != TILE_WATER && currTile->GetCurrentTileType() != TILE_LAVA) {
			position = currTile->GetLocation();
			foundALocation = true;
		}


		if (numTimesRan > 10000) {
			DebuggerPrintf("ERROR: Find Random Valid Location running too long.");
			return IntVector2(0, 0);
		}
		numTimesRan++;
	}

	return position;
}
//---------------------------------------------------------------------------------------------------------------------------
std::vector<IntVector2> Map::GetValidLocsToMoveToAroundLoc(const IntVector2& loc, int movementProperties) {

	std::vector<IntVector2> possibleLocs;

	possibleLocs.push_back(loc + NORTH);
	possibleLocs.push_back(loc + NORTHEAST);
	possibleLocs.push_back(loc + EAST);
	possibleLocs.push_back(loc + SOUTHEAST);
	possibleLocs.push_back(loc + SOUTH);
	possibleLocs.push_back(loc + SOUTHWEST);
	possibleLocs.push_back(loc + WEST);
	possibleLocs.push_back(loc + NORTHWEST);

	std::vector<IntVector2> locs;
	for (unsigned int i = 0; i < possibleLocs.size(); i++) {
		IntVector2 currLoc = possibleLocs[i];

		if (GetTileAtLocation(currLoc) && GetTileAtLocation(currLoc)->IsValidForAgent(movementProperties)) {
			locs.push_back(currLoc);
		}
	}

	return locs;
}

int Map::GetManhattanDistance(const IntVector2& start, const IntVector2& end) {
	return abs(end.x - start.x) + abs(end.y - start.y);
}

IntVector2 Map::GetLocationWithOpeningOnEitherSide() {

	std::vector<Tile*> allStoneTilesOnMap;

	for (int x = 0; x < m_size.x; x++) {
		for (int y = 0; y < m_size.y; y++) {
			IntVector2 loc = IntVector2(x, y);
			Tile* currTile = GetTileAtLocation(loc);

			if (currTile->GetCurrentTileType() == TILE_STONE) {
				allStoneTilesOnMap.push_back(currTile);
			}
		}
	}

	int numTimesRan = 0;
	bool b = true;
	while (b) {
		int which = RandIntZeroToSize(allStoneTilesOnMap.size());

		Tile*& currTile = allStoneTilesOnMap[which];
		IntVector2 loc = currTile->GetLocation();

		Tile* tileToLeft = GetTileAtLocation(loc + WEST);
		Tile* tileToRight = GetTileAtLocation(loc + EAST);
		Tile* tileToUp = GetTileAtLocation(loc + NORTH);
		Tile* tileToDown = GetTileAtLocation(loc + SOUTH);

		if (tileToLeft && tileToRight && tileToLeft->IsValid() && tileToRight->IsValid()) {
			return loc;
		}
		else if (tileToUp && tileToDown && tileToUp->IsValid() && tileToDown->IsValid()) {
			return loc;
		}


		if (numTimesRan > 10000) {
			DebuggerPrintf("ERROR: Find Random Valid Location running too long.");
			return IntVector2(0, 0);
		}
		numTimesRan++;
	}

	return IntVector2(0, 0);
}

STATIC bool Map::IsLocAdjacentToLoc(const IntVector2& loc1, const IntVector2& loc2) {
	int dist = abs(loc1.x - loc2.x) + abs(loc1.y - loc2.y);

	return (dist == 1 || dist == 2);
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVING LOADING
//---------------------------------------------------------------------------------------------------------------------------
XMLNode Map::GetMapLegend() {
	XMLNode legendNode = legendNode.createXMLTopNode("Legend");

	XMLNode airTileLegendNode = airTileLegendNode.createXMLTopNode("Tile");
	airTileLegendNode.addAttribute("type", "air");
	airTileLegendNode.addAttribute("char", "0");
	XMLNode stoneTileLegendNode = stoneTileLegendNode.createXMLTopNode("Tile");
	stoneTileLegendNode.addAttribute("type", "stone");
	stoneTileLegendNode.addAttribute("char", "#");
	XMLNode grassTileLegendNode = grassTileLegendNode.createXMLTopNode("Tile");
	grassTileLegendNode.addAttribute("type", "grass");
	grassTileLegendNode.addAttribute("char", ".");
	XMLNode waterTileLegendNode = waterTileLegendNode.createXMLTopNode("Tile");
	waterTileLegendNode.addAttribute("type", "water");
	waterTileLegendNode.addAttribute("char", "$");
	XMLNode lavaTileLegendNode = lavaTileLegendNode.createXMLTopNode("Tile");
	lavaTileLegendNode.addAttribute("type", "lava");
	lavaTileLegendNode.addAttribute("char", "x");

	legendNode.addChild(airTileLegendNode);
	legendNode.addChild(stoneTileLegendNode);
	legendNode.addChild(grassTileLegendNode);
	legendNode.addChild(waterTileLegendNode);
	legendNode.addChild(lavaTileLegendNode);

	return legendNode;
}
String Map::GetMapDataAsText() {

	String mapDataAsText = "";
	mapDataAsText.push_back('\n');

	for (int y = m_size.y - 1; y >= 0; y--) {
		for (int x = 0; x < m_size.x; x++) {
			Tile* tile = GetTileAtLocation(IntVector2(x, y));

			switch (tile->GetCurrentTileType()) {
			case TILE_AIR:
				mapDataAsText.push_back('0');
				break;
			case TILE_STONE:
				mapDataAsText.push_back('#');
				break;
			case TILE_GRASS:
				mapDataAsText.push_back('.');
				break;
			case TILE_WATER:
				mapDataAsText.push_back('$');
				break;
			case TILE_LAVA:
				mapDataAsText.push_back('x');
				break;
			}
		}

		mapDataAsText.push_back('\n');
	}

	//String mapData = StringUtils::ReverseString(mapDataAsText);

	return mapDataAsText;
}
String Map::GetVisibilityAsText() {

	String visDataAsText = "";

	visDataAsText.push_back('\n');

	for (int y = m_size.y - 1; y >= 0; y--) {
		for (int x = 0; x < m_size.x; x++) {
			Tile* tile = GetTileAtLocation(IntVector2(x, y));


			if (tile->IsVisible()) {
				visDataAsText.push_back('*');
			}
			else if (tile->IsExplored()) {
				visDataAsText.push_back('.');
			}
			else {
				visDataAsText.push_back('#');
			}
		}

		visDataAsText.push_back('\n');
	}

	return visDataAsText;
}
XMLNode Map::SaveToXML() {

	XMLNode mapData = mapData.createXMLTopNode("MapData");
	mapData.addAttribute("size", m_size.ToString().c_str());

	XMLNode legendData = GetMapLegend();

	XMLNode tileData = tileData.createXMLTopNode("TileData");
	tileData.addText(GetMapDataAsText().c_str());
	
	XMLNode visiblityData = visiblityData.createXMLTopNode("VisibilityData");
	visiblityData.addText(GetVisibilityAsText().c_str());

	mapData.addChild(legendData);
	mapData.addChild(tileData);
	mapData.addChild(visiblityData);
	
	return mapData;
}