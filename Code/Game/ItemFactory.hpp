#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Item.hpp"
#include "Game/Map.hpp"

class ItemFactory {
public:
	//STRUCTORS
	ItemFactory(const XMLNode& itemBlueprintNode);
	void PopulateFromXMLNode(const XMLNode& itemBlueprintNode);

	//CREATION
	Item* Create(Map* belongingMap, const IntVector2& loc);

	//GETTERS SETTERS
	String GetName() const { return m_name; }

	//STATIC
	static int GetNumFactories() { return s_itemFactories.size(); }
	static ItemFactory* GetRandomFactory();
	static void LoadAllItems();
	static ItemFactory* GetFactoryByName(const String& name);

private:
	Item* m_templateItem;
	String m_name;

	static std::map<String, ItemFactory*> s_itemFactories;
};