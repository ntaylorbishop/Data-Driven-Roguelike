#include "Game/FromDataGenerator.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
FromDataGenerator::FromDataGenerator(EnvironmentGeneratorData* genData)
	: Generator(genData)
{ 
	FromDataGeneratorData* dataGenData = static_cast<FromDataGeneratorData*>(m_generatorData);

	String fileName = dataGenData->GetFileName();


	XMLNode root = XMLNode::parseFile(fileName.c_str());
	ParseData(root);
}
void FromDataGenerator::ParseData(const XMLNode& root) {
	XMLNode mapNode = root.getChildNode(0);

	//Parse legend
	XMLNode legendNode = mapNode.getChildNode(0);
	for (int i = 0; i < legendNode.nChildNode(); i++) {
		XMLNode legendChar = legendNode.getChildNode(i);

		XMLAttribute charAttrib = legendChar.getAttribute(0);
		XMLAttribute typeAttrib = legendChar.getAttribute(1);

		char rep = XMLUtils::ParseCharFromXML(charAttrib.lpszValue);
		eTileType tile = Tile::ParseTileTypeFromString(typeAttrib.lpszValue);

		m_legendNodes.push_back(LegendNode(rep, tile));
	}

	//Parse map
	XMLNode mapDataNode = mapNode.getChildNode(1);

	String rawMapData = mapDataNode.getText();
	rawMapData = StringUtils::GetRidOfAllOneCharacter(rawMapData, ' ');
	m_mapData = StringUtils::GetRidOfAllOneCharacter(rawMapData, '\r');
}

//---------------------------------------------------------------------------------------------------------------------------
//GENERATION
//---------------------------------------------------------------------------------------------------------------------------
Map* FromDataGenerator::GenerateMap(const IntVector2& size) {
	Map* dataMap = ParseDataIntoTiles();
	Map* nMap = new Map(size);

	IntVector2 nMapSize = size;
	IntVector2 dataMapSize = dataMap->GetSize();

	IntVector2 startLoc = nMapSize - dataMapSize;
	startLoc = startLoc / 2;

	IntVector2 dataMapLoc = IntVector2(0, 0);

	for (int x = 0; x < dataMapSize.x; x++) {
		for (int y = 0; y < dataMapSize.y; y++) {
			IntVector2 currLocOnDataMap = IntVector2(x, y);
			IntVector2 currLocOnNewMap = currLocOnDataMap + startLoc;

			Tile* currTileOnDataMap = dataMap->GetTileAtLocation(currLocOnDataMap);
			Tile* currTileOnNewMap = nMap->GetTileAtLocation(currLocOnNewMap);

			currTileOnNewMap->SetCurrentTileType(currTileOnDataMap->GetCurrentTileType());
		}
	}

	return nMap;
}
bool FromDataGenerator::GenerateStep(Map* map, int& currentStepNumber) {
	//Take current map and place data map
	UNREFERENCED_PARAMETER(map);
	UNREFERENCED_PARAMETER(currentStepNumber);
	return true;
}

void FromDataGenerator::GenerateAll(Map* map, int numSteps) {
	UNREFERENCED_PARAMETER(map);
	UNREFERENCED_PARAMETER(numSteps);
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
Map* FromDataGenerator::ParseDataIntoTiles() {
	std::vector<Tile> m_tiles;

	//Get rows and reverse
	std::vector<String> rows;
	StringUtils::TokenizeBuffer(m_mapData, '\n', rows);
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

	return dataMap;
}
eTileType FromDataGenerator::GetTileTypeFromLegend(char currTileChar) {
	for (unsigned int i = 0; i < m_legendNodes.size(); i++) {
		if (currTileChar == m_legendNodes[i].m_representation) {
			return m_legendNodes[i].m_tileType;
		}
	}

	return TILE_GRASS;
}
//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
GeneratorRegistration FromDataGenerator::s_fromDataRegistration("FromData", &FromDataGenerator::CreateGenerator);

STATIC Generator* FromDataGenerator::CreateGenerator(EnvironmentGeneratorData* genData) {
	return new FromDataGenerator(genData);
}