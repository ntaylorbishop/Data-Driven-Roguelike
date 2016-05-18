#include "Game/NPCFactory.hpp"

STATIC std::map<String, NPCFactory*> NPCFactory::s_npcFactories;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
NPCFactory::NPCFactory(const XMLNode& npcBlueprintNode) {
	PopulateFromXMLNode(npcBlueprintNode);
}
void NPCFactory::PopulateFromXMLNode(const XMLNode& npcBlueprintNode) {

	String name = NPC_DEFAULT_NAME;
	RangeInt maxHealth = NPC_DEFAULT_HEALTH;

	std::vector<XMLUtils::Attribute> npcAttribs;
	XMLUtils::GetAllAttributesOfNode(npcBlueprintNode, npcAttribs);

	for (unsigned int i = 0; i < npcAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = npcAttribs[i];
		StringUtils::ToLowerCase(currAttrib.name);
		
		if (currAttrib.name == "name") {
			name = currAttrib.value;
		}
		else if (currAttrib.name == "maxhealth") {
			maxHealth = XMLUtils::ParseRangeIntFromXML(currAttrib.value);
		}
	}

	m_name = name;
	m_maxHealth = maxHealth;

	m_templateNPC = new NPC(npcBlueprintNode);
	m_templateNPC->SetName(m_name);
	m_templateNPC->SetMaxHealth(m_maxHealth.Roll());
}

//---------------------------------------------------------------------------------------------------------------------------
//CREATION
//---------------------------------------------------------------------------------------------------------------------------
NPC* NPCFactory::Create(Map* belongingMap, const IntVector2& loc) {
	NPC* newNPC = new NPC(m_templateNPC);
	newNPC->SetMaxHealth(m_maxHealth.Roll());
	newNPC->SetHealth(newNPC->GetMaxHealth());
	newNPC->SetPosition(loc);
	newNPC->SetBelongingMap(belongingMap);

	return newNPC;
}
STATIC NPCFactory* NPCFactory::GetRandomFactory() {
	std::map<String, NPCFactory*>::iterator item = s_npcFactories.begin();
	std::advance(item, RandInt(0, s_npcFactories.size() - 1));
	return item->second;
}

//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
STATIC void NPCFactory::LoadAllNPCs() {
	String relDir = "Data/NPCBlueprints/";
	std::vector<String> filePaths = FileUtils::GetAllFilenamesInDirectory(relDir);

	for (unsigned int i = 0; i < filePaths.size(); i++) {
		XMLNode root = XMLNode::parseFile(filePaths[i].c_str());
		root = root.getChildNode(0);

		for (int j = 0; j < root.nChildNode(); j++) {
			XMLNode npcBlueprint = root.getChildNode(j);

			NPCFactory* currFactory = new NPCFactory(npcBlueprint);
			s_npcFactories.insert(std::pair<String, NPCFactory*>(currFactory->GetName(), currFactory));
		}
	}
}
STATIC NPCFactory* NPCFactory::CreateByName(const String& name) {
	std::map<String, NPCFactory*>::iterator factory = s_npcFactories.find(name);

	if (factory == s_npcFactories.end()) {
		DebuggerPrintf("%s%s", "Couldn't find NPC blueprint ", name);
		return nullptr;
	}
	else {
		return factory->second;
	}
}