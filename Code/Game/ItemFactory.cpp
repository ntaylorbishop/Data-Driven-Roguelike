#include "Game/ItemFactory.hpp"

STATIC std::map<String, ItemFactory*> ItemFactory::s_itemFactories;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
ItemFactory::ItemFactory(const XMLNode& itemBlueprintNode) {
	PopulateFromXMLNode(itemBlueprintNode);
}
void ItemFactory::PopulateFromXMLNode(const XMLNode& itemBlueprintNode) {
	String name = ITEM_DEFAULT_NAME;

	std::vector<XMLUtils::Attribute> npcAttribs;
	XMLUtils::GetAllAttributesOfNode(itemBlueprintNode, npcAttribs);

	for (unsigned int i = 0; i < npcAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = npcAttribs[i];
		StringUtils::ToLowerCase(currAttrib.name);

		if (currAttrib.name == "name") {
			name = currAttrib.value;
		}
	}

	m_name = name;

	m_templateItem = new Item(itemBlueprintNode);
}

//---------------------------------------------------------------------------------------------------------------------------
//CREATION
//---------------------------------------------------------------------------------------------------------------------------
Item* ItemFactory::Create(Map* belongingMap, const IntVector2& loc) {
	Item* newItem = new Item(m_templateItem);
	newItem->SetPosition(loc);
	newItem->SetBelongingMap(belongingMap);

	return newItem;
}

//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
STATIC ItemFactory* ItemFactory::GetRandomFactory() {
	std::map<String, ItemFactory*>::iterator item = s_itemFactories.begin();
	std::advance(item, RandInt(0, s_itemFactories.size() - 1));
	return item->second;
}
STATIC void ItemFactory::LoadAllItems() {
	String relDir = "Data/Items/";
	std::vector<String> filePaths = FileUtils::GetAllFilenamesInDirectory(relDir);

	for (unsigned int i = 0; i < filePaths.size(); i++) {
		XMLNode root = XMLNode::parseFile(filePaths[i].c_str());
		root = root.getChildNode(0);

		for (int j = 0; j < root.nChildNode(); j++) {
			XMLNode itemBlueprint = root.getChildNode(j);

			ItemFactory* currFactory = new ItemFactory(itemBlueprint);
			s_itemFactories.insert(std::pair<String, ItemFactory*>(currFactory->GetName(), currFactory));
		}
	}
}
STATIC ItemFactory* ItemFactory::GetFactoryByName(const String& name) {
	std::map<String, ItemFactory*>::iterator factory = s_itemFactories.find(name);

	if (factory == s_itemFactories.end()) {
		DebuggerPrintf("%s%s", "Couldn't find Item blueprint ", name);
		return nullptr;
	}
	else {
		return factory->second;
	}
}