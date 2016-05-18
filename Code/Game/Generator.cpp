#include "Game/Generator.hpp"
#include "Game/TheWorld.hpp"

STATIC std::map<String, GeneratorRegistration*>* GeneratorRegistration::s_generatorRegistrationMap = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//GENERATOR
//---------------------------------------------------------------------------------------------------------------------------
Generator::Generator(EnvironmentGeneratorData* genData) 
	: m_generatorData(genData)
{ }

Map* Generator::GenerateMap(const IntVector2& size) {
	UNREFERENCED_PARAMETER(size);

	return new Map;
}

//---------------------------------------------------------------------------------------------------------------------------
//GENERATOR REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
GeneratorRegistration::GeneratorRegistration(const String& name, GeneratorCreationFunc* creationFunc)
	: m_name(name)
	, m_creationFunc(creationFunc)
{
	if (!s_generatorRegistrationMap) {
		s_generatorRegistrationMap = new std::map<String, GeneratorRegistration*>;
	}

	std::map<String, GeneratorRegistration*>::iterator findGeneratorIt = s_generatorRegistrationMap->find(name);

	if (findGeneratorIt == s_generatorRegistrationMap->end()) {
		s_generatorRegistrationMap->insert(std::pair<String, GeneratorRegistration*>(name, this));
	}
	else {
		String str = "ERROR: " + name + "ALREADY EXISTS IN MAP REGISTRATION";
		DebuggerPrintf(str.c_str());
	}

}

STATIC Generator* GeneratorRegistration::CreateGeneratorByName(const String& name, EnvironmentGeneratorData* genData) {
	ASSERT_OR_DIE(genData != nullptr, "ERROR: MUST SUPPLY GENERATOR DATA WHEN CREATING A GENERATOR");
	ASSERT_OR_DIE(s_generatorRegistrationMap != nullptr, "GENERATOR REGISTRATION MAP IS NULL");

	std::map<String, GeneratorRegistration*>::iterator findNameIt = s_generatorRegistrationMap->find(name);
	ASSERT_OR_DIE(findNameIt != s_generatorRegistrationMap->end(), "ERROR: GENERATOR DOES NOT EXIST");

	GeneratorRegistration* genRegistration = findNameIt->second;
	Generator* generator = (*genRegistration->m_creationFunc)(genData);

	return generator;
}

//---------------------------------------------------------------------------------------------------------------------------
//STATIC FUNCTIONALITY
//---------------------------------------------------------------------------------------------------------------------------
STATIC void Generator::FinalizeMap(Map* map) {
	IntVector2 mapSize = map->GetSize();

	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			IntVector2 currLoc = IntVector2(x, y);
			Tile* currTile = map->GetTileAtLocation(currLoc);

			if (nullptr != currTile) {
				currTile->SetFutureTileType(currTile->GetCurrentTileType());
			}
		}
	}

	//Make all hidden tiles air
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			IntVector2 currLoc = IntVector2(x, y);

			if (map->AreNeighborsAllSolid(currLoc)) {
				Tile* currTile = map->GetTileAtLocation(currLoc);
				if (nullptr != currTile) {
					currTile->SetFutureTileType(TILE_AIR);
				}
			}
		}
	}

	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			IntVector2 currLoc = IntVector2(x, y);
			Tile* currTile = map->GetTileAtLocation(currLoc);

			currTile->SetCurrentTileType(currTile->GetFutureTileType());
		}
	}

	//Walk perimeter and make all solid
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			IntVector2 currLoc = IntVector2(x, y);
			Tile* currTile = map->GetTileAtLocation(currLoc);

			if (currLoc.x == 0 || currLoc.y == 0 || currLoc.x == mapSize.x - 1 || currLoc.y == mapSize.y - 1) {
				if (nullptr != currTile && currTile->GetCurrentTileType() != TILE_AIR) {
					currTile->SetCurrentTileType(TILE_STONE);
				}
			}
		}
	}

	//Walk map and grab all water tiles
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			IntVector2 currLoc = IntVector2(x, y);
			Tile* currTile = map->GetTileAtLocation(currLoc);

			if (nullptr != currTile && currTile->IsDynamicallyRendered()) {
				map->m_dynamicallyRenderedTiles.push_back(currTile);
			}
		}
	}

	PopulateMap(map);
}
//---------------------------------------------------------------------------------------------------------------------------
STATIC void Generator::FinalizeLoadedMap(Map* map) {
	IntVector2 mapSize = map->GetSize();

	////////////////////////////////////////////////////
	//------------------------------------------
	//GET DYNAMICALLY RENDERED TILES
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			IntVector2 currLoc = IntVector2(x, y);
			Tile* currTile = map->GetTileAtLocation(currLoc);

			if (nullptr != currTile && currTile->IsDynamicallyRendered()) {
				map->m_dynamicallyRenderedTiles.push_back(currTile);
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
STATIC void Generator::PopulateMap(Map* map) {
	////////////////////////////////////////////////////
	//------------------------------------------
	//NPCS
	std::vector<IntVector2> npcLocs;

	while (npcLocs.size() < NUM_ENEMIES_ON_MAP) {
		IntVector2 potentialLoc = map->FindRandomValidLocationToSpawn();

		bool potentialLocAlreadyPicked = false;
		for (unsigned int i = 0; i < npcLocs.size(); i++) {
			if (npcLocs[i] == potentialLoc) {
				potentialLocAlreadyPicked = true;
			}
		}

		if (!potentialLocAlreadyPicked) {
			npcLocs.push_back(potentialLoc);
		}
	}

	for (unsigned int i = 0; i < npcLocs.size(); i++) {
		NPCFactory* currFactory = NPCFactory::GetRandomFactory();

		g_theWorld->m_entitiesOnMap.push_back(currFactory->Create(map, npcLocs[i]));
	}

	////////////////////////////////////////////////////
	//------------------------------------------
	//ITEMS
	std::vector<IntVector2> itemLocs;

	while (itemLocs.size() < NUM_ITEMS_ON_MAP) {
		IntVector2 potentialLoc = map->FindRandomValidLocationToSpawn();

		bool potentialLocAlreadyPicked = false;
		for (unsigned int i = 0; i < itemLocs.size(); i++) {
			if (itemLocs[i] == potentialLoc) {
				potentialLocAlreadyPicked = true;
			}
		}

		if (!potentialLocAlreadyPicked) {
			itemLocs.push_back(potentialLoc);
		}
	}

	for (unsigned int i = 0; i < itemLocs.size(); i++) {
		ItemFactory* currFactory = ItemFactory::GetRandomFactory();

		Item* newItem = currFactory->Create(map, itemLocs[i]);
		g_theWorld->AddItemToEntityList(newItem);

		Inventory* inv = new Inventory();
		inv->PutItemInBag(newItem);
		g_theWorld->m_activeMap->GetTileAtLocation(itemLocs[i])->SetInventory(inv);
	}

	////////////////////////////////////////////////////
	//------------------------------------------
	//FEATURES
	for (unsigned int i = 1; i < 4; i++) {
		FeatureFactory* currFactory = FeatureFactory::GetRandomFactory();

		IntVector2 locToPlace = map->GetLocationWithOpeningOnEitherSide();
		Feature* addedFeature = currFactory->Create(map, locToPlace);
		g_theWorld->m_entitiesOnMap.push_back(addedFeature);
		map->GetTileAtLocation(locToPlace)->SetFeature(addedFeature);
	}

	////////////////////////////////////////////////////
	//------------------------------------------
	//REFRESH COMBAT SYSTEM
	if (g_theCombatSystem) {
		delete g_theCombatSystem;
		g_theCombatSystem = nullptr;
	}

	g_theCombatSystem = new TheCombatSystem(g_theWorld->m_entitiesOnMap);
}