#pragma once

#include <iterator>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/FileUtils.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "ThirdParty/Parsers/xmlParser.h"
#include "Game/GameCommon.hpp"
#include "Game/NPC.hpp"

class NPCFactory {
public:
	//STRUCTORS
	NPCFactory(const XMLNode& npcBlueprintNode);
	void PopulateFromXMLNode(const XMLNode& npcBlueprintNode);
	
	//CREATION
	NPC* Create(Map* belongingMap, const IntVector2& loc);
	static NPCFactory* GetRandomFactory();

	//GETTERS SETTERS
	String GetName() const { return m_name; }
	static int GetNumFactories() { return s_npcFactories.size(); }

	//STATIC
	static void LoadAllNPCs();
	static NPCFactory* CreateByName(const String& name);

private:
	NPC* m_templateNPC;
	String m_name;
	RangeInt m_maxHealth;

	static std::map<String, NPCFactory*> s_npcFactories;
};