#include "Game/EnvironmentBlueprint.hpp"

STATIC std::map<String, EnvironmentBlueprint*> EnvironmentBlueprint::s_loadedEnvironmentBlueprints;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS / INITIALIZATION
//---------------------------------------------------------------------------------------------------------------------------
EnvironmentBlueprint::EnvironmentBlueprint(const XMLNode& root)
	: m_name("NULL")
	, m_size(IntVector2(-1, -1))
{
	PopulateFromXMLNode(root);
}
void EnvironmentBlueprint::PopulateFromXMLNode(const XMLNode& root) {
	String nameNode = root.getAttribute(0).lpszValue;
	String sizeNode = root.getAttribute(1).lpszValue;
	IntVector2 size = XMLUtils::ParseIntVector2FromXML(sizeNode);

	m_name = nameNode;
	m_size = size;


	for (int childNodeIdx = 0; childNodeIdx < root.nChildNode(); childNodeIdx++) {
		XMLNode child = root.getChildNode(childNodeIdx);

		String envGenDataAttribName = child.getAttribute(0).lpszName;
		String envGenDataName = child.getAttribute(0).lpszValue;

		ASSERT_OR_DIE(envGenDataAttribName == "generator", "ERROR: BAD ENVIRONMENT XML DATA");

		EnvironmentGeneratorData* currGenData = EnvironmentGeneratorDataRegistration::CreateEGPDByName(envGenDataName, child);
		m_genDatas.push_back(currGenData);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
String EnvironmentBlueprint::GetName() const {
	return m_name;
}
IntVector2 EnvironmentBlueprint::GetSize() const {
	return m_size;
}

//---------------------------------------------------------------------------------------------------------------------------
//LOAD IN ENVIRONMENT BLUEPRINTS FROM XML FILES
//---------------------------------------------------------------------------------------------------------------------------
STATIC void EnvironmentBlueprint::LoadEnvironmentBlueprints() {
	String relDir = "Data/Environments/";
	std::vector<String> filePaths = FileUtils::GetAllFilenamesInDirectory(relDir);

	for (unsigned int i = 0; i < filePaths.size(); i++) {
		XMLNode root = XMLNode::parseFile(filePaths[i].c_str());

		root = root.getChildNode(0);
		ASSERT_OR_DIE(!root.isEmpty(), "ERROR: Could not load environment file " + filePaths[i]);
		EnvironmentBlueprint* currBlueprint = new EnvironmentBlueprint(root);
		s_loadedEnvironmentBlueprints.insert(std::pair<String, EnvironmentBlueprint*>(currBlueprint->GetName(), currBlueprint));
	}
}

STATIC EnvironmentBlueprint* EnvironmentBlueprint::GetEnvironmentBlueprintByName(const String& name) {
	std::map<String, EnvironmentBlueprint*>::iterator blueprint = s_loadedEnvironmentBlueprints.find(name);

	if (blueprint == s_loadedEnvironmentBlueprints.end()) {
		DebuggerPrintf("%s%s", "Couldn't find environment blueprint ", name);
		return nullptr;
	}
	else {
		return blueprint->second;
	}
}

