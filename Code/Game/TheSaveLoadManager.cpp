#include "Game/TheSaveLoadManager.hpp"
#include "Game/TheWorld.hpp"
#include "Game/TheMessageLog.hpp"
#include "Game/TheInfoLog.hpp"

TheSaveLoadManager* g_theSaveLoadManager = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
TheSaveLoadManager::TheSaveLoadManager() 
	: m_filePath("Data/SaveGame/SaveGame.xml")
	, m_saveGameExists(false)
{ 
	if (FileUtils::DoesFileExist(m_filePath)) {
		m_saveGameExists = true;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVE
//---------------------------------------------------------------------------------------------------------------------------
void TheSaveLoadManager::SaveGame() {

	XMLNode rootNode = XMLNode();
	rootNode = rootNode.createXMLTopNode("SaveGame");

	XMLNode mapData = g_theWorld->m_activeMap->SaveToXML();
	XMLNode entityData = g_theWorld->SaveEntityData();
	XMLNode turnData = g_theCombatSystem->SaveToXML();
	XMLNode msgData = g_theMessageLog->SaveToXML();


	rootNode.addChild(mapData);
	rootNode.addChild(entityData);
	rootNode.addChild(turnData);
	rootNode.addChild(msgData);

	if (rootNode.writeToFile(m_filePath.c_str()) == 0) {
		g_theInfoLog->PlaySaveAnimation(true);
	}
	else {
		g_theInfoLog->PlaySaveAnimation(false);
	}
}

////////////////////////////////////////////////////
//------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//LOAD
//---------------------------------------------------------------------------------------------------------------------------
void TheSaveLoadManager::ParseMapData(const XMLNode& mapDataNode) {

	String mapDataSizeStr = mapDataNode.getAttribute(0).lpszValue;
	m_loadedMapSize = XMLUtils::ParseIntVector2FromXML(mapDataSizeStr);

	for (int i = 0; i < mapDataNode.nChildNode(); i++) {
		XMLNode mapDataChildNode = mapDataNode.getChildNode(i);

		String nodeName = mapDataChildNode.getName();

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE LEGEND
		if (nodeName == "Legend") {
			ParseLegend(mapDataChildNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE TILE DATA
		else if (nodeName == "TileData") {
			ParseTileData(mapDataChildNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE VIS DATA
		else if (nodeName == "VisibilityData") {
			ParseVisibilityData(mapDataChildNode);
		}
	}
}
void TheSaveLoadManager::ParseLegend(const XMLNode& legendNode) {
	for (int i = 0; i < legendNode.nChildNode(); i++) {
		XMLNode legendChar = legendNode.getChildNode(i);

		XMLAttribute typeAttrib = legendChar.getAttribute(0);
		XMLAttribute charAttrib = legendChar.getAttribute(1);

		char rep = XMLUtils::ParseCharFromXML(charAttrib.lpszValue);
		eTileType tile = Tile::ParseTileTypeFromString(typeAttrib.lpszValue);

		m_legendNodes.push_back(LegendNode(rep, tile));
	}
}
void TheSaveLoadManager::ParseTileData(const XMLNode& tileDataNode) {

	//Get the tile data and refresh the map
	String mapData = tileDataNode.getText();
	g_theWorld->m_activeMap = new Map(m_loadedMapSize);

	//Parse it into a new map
	//Get rows and reverse
	std::vector<String> rows;
	StringUtils::TokenizeBuffer(mapData, '\n', rows);
	std::vector<String> reversedRows;
	for (int i = (int)rows.size() - 1; i > -1; i--) {
		reversedRows.push_back(rows[i]);
	}

	//Get size of map and create it
	String currRow = rows[0];
	for (unsigned int i = 1; i < rows.size(); i++) {
		ASSERT_OR_DIE(rows[i].size() == currRow.size(), "ERROR: BAD XML MAP DATA FOR FROM DATA GENERATOR");
		currRow = rows[i];
	}
	int ySize = rows.size();

	Map* dataMap = new Map(IntVector2(rows[0].size(), ySize));

	for (unsigned int y = 0; y < reversedRows.size(); y++) {
		String currRowLoop = reversedRows[y];

		for (unsigned int x = 0; x < currRowLoop.size(); x++) {
			char currTileChar = currRowLoop[x];
			IntVector2 loc = IntVector2(x, y);
			eTileType currTileType = GetTileTypeFromLegend(currTileChar);

			Tile* currTile = dataMap->GetTileAtLocation(loc);
			currTile->SetCurrentTileType(currTileType);
		}
	}

	g_theWorld->m_activeMap = dataMap;

}
eTileType TheSaveLoadManager::GetTileTypeFromLegend(char currTileChar) {
	for (unsigned int i = 0; i < m_legendNodes.size(); i++) {
		if (currTileChar == m_legendNodes[i].m_representation) {
			return m_legendNodes[i].m_tileType;
		}
	}

	return TILE_AIR;
}
bool TheSaveLoadManager::IsTileDataVisible(char c) const {
	return (c == '*') ? (true) : (false);
}
bool TheSaveLoadManager::IsTileDataExplored(char c) const {
	return (c == '.') ? (true) : (false);
}
void TheSaveLoadManager::ParseVisibilityData(const XMLNode& visDataNode) {

	//Get the tile data and refresh the map
	String mapData = visDataNode.getText();

	//Parse it into a new map
	//Get rows and reverse
	std::vector<String> rows;
	StringUtils::TokenizeBuffer(mapData, '\n', rows);
	std::vector<String> reversedRows;
	for (int i = (int)rows.size() - 1; i > -1; i--) {
		reversedRows.push_back(rows[i]);
	}

	//Get size of map and create it
	String currRow = rows[0];
	for (unsigned int i = 1; i < rows.size(); i++) {
		ASSERT_OR_DIE(rows[i].size() == currRow.size(), "ERROR: BAD XML MAP DATA FOR FROM DATA GENERATOR");
		currRow = rows[i];
	}

	Map* dataMap = g_theWorld->m_activeMap;

	for (unsigned int y = 0; y < reversedRows.size(); y++) {
		String currRowLoop = reversedRows[y];

		for (unsigned int x = 0; x < currRowLoop.size(); x++) {
			char currTileChar = currRowLoop[x];
			IntVector2 loc = IntVector2(x, y);

			bool isVisible = IsTileDataVisible(currTileChar);
			bool isExplored = IsTileDataExplored(currTileChar);

			Tile* currTile = dataMap->GetTileAtLocation(loc);
			currTile->SetVisibility(isVisible);
			
			if (isExplored) {
				currTile->Explore();
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////
//------------------------------------------
void TheSaveLoadManager::ParseEntityData(const XMLNode& entityDataNode) {
	g_theWorld->m_thePlayer = new Player(entityDataNode.getChildNode(0));
	g_theWorld->m_entitiesOnMap.push_back(g_theWorld->m_thePlayer);

	if (entityDataNode.nChildNode() == 1) {
		return;
	}
	
	for (int i = 0; i < entityDataNode.nChildNode(); i++) {
	
		XMLNode currNode = entityDataNode.getChildNode(i);
		String name = currNode.getName();

		if (name == "NPCBlueprint") {
			NPC* newNPC = new NPC();
			newNPC->LoadFromXML(currNode);
			g_theWorld->m_entitiesOnMap.push_back(newNPC);
		}
		else if (name == "Inventory") {
			Inventory* inv = new Inventory();
			inv->LoadFromXML(currNode);
		}
		else if (name == "FeatureBlueprint") {
			Feature* newFeature = new Feature();
			newFeature->LoadFromXML(currNode);
			g_theWorld->m_entitiesOnMap.push_back(newFeature);
		}
	}

	ResolveEntityPointers();
}
//---------------------------------------------------------------------------------------------------------------------------
void TheSaveLoadManager::ResolveEntityPointers() {
	for (unsigned int i = 0; i < g_theWorld->m_entitiesOnMap.size(); i++) {
		g_theWorld->m_entitiesOnMap[i]->ResolveEntityPointers();
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TheSaveLoadManager::LoadGame() {
	//DEBUG DELETE PLAYER
	if (g_theWorld->m_thePlayer) {
		delete g_theWorld->m_thePlayer;
		g_theWorld->m_thePlayer = nullptr;
	}
	if (g_theWorld->m_activeMap) {
		delete g_theWorld->m_activeMap;
		g_theWorld->m_activeMap = nullptr;
	}

	XMLNode root = XMLNode::parseFile(m_filePath.c_str());
	root = root.getChildNode(0);

	for (int i = 0; i < root.nChildNode(); i++) {
		XMLNode currNode = root.getChildNode(i);

		String nodeName = currNode.getName();

		////////////////////////////////////////////////////
		//------------------------------------------
		//MAP DATA
		if (nodeName == "MapData") {
			ParseMapData(currNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//ENTITY DATA
		if (nodeName == "EntityData") {
			ParseEntityData(currNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE TURN DATA
		else if (nodeName == "MessageLog") {
			TheMessageLog::LoadFromXML(currNode);
		}

		////////////////////////////////////////////////////
		//------------------------------------------
		//PARSE MSG DATA
		else if (nodeName == "TurnInfo") {
			TheCombatSystem::LoadFromXML(currNode);
		}
	}
}