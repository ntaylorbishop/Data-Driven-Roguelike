#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/General/Utils/FileUtils.hpp"
#include "Game/FromDataGenerator.hpp"

class TheSaveLoadManager;
extern TheSaveLoadManager* g_theSaveLoadManager;

class TheSaveLoadManager {
public:
	//STRUCTORS
	TheSaveLoadManager();

	//SAVE
	void SaveGame();

	//LOAD
	void LoadGame();

	//GETTERS SETTERS
	bool DoesSaveGameExist() const { return m_saveGameExists; }

private:
	//PARSING MAP DATA
	void ParseMapData(const XMLNode& mapDataNode);
	void ParseLegend(const XMLNode& legendNode);
	void ParseTileData(const XMLNode& tileDataNode);
	void ParseVisibilityData(const XMLNode& visDataNode);

	eTileType GetTileTypeFromLegend(char currTileChar);
	bool IsTileDataVisible(char c) const;
	bool IsTileDataExplored(char c) const;

	//PARSING ENTITY DATA
	void ParseEntityData(const XMLNode& entityDataNode);
	void ResolveEntityPointers();


	String m_filePath;
	bool m_saveGameExists;
	std::vector<LegendNode> m_legendNodes;
	IntVector2 m_loadedMapSize;
};