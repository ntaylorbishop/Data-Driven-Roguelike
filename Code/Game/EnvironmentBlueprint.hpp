#pragma once

#include <map>
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/FileUtils.hpp"
#include "ThirdParty/Parsers/xmlParser.h"
#include "Game/EnvironmentGeneratorData.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"

class EnvironmentBlueprint {
public:
	//STRUCTORS / INITIALIZATION
	EnvironmentBlueprint(const XMLNode& root);
	void PopulateFromXMLNode(const XMLNode& root);

	//GETTERS SETTERS
	String GetName() const;
	IntVector2 GetSize() const;

	std::vector<EnvironmentGeneratorData*> m_genDatas;

private:
	String m_name;
	IntVector2 m_size;




public:
	//LOAD IN ENVIRONMENT BLUEPRINTS FROM XML FILES
	static void LoadEnvironmentBlueprints();
	static EnvironmentBlueprint* GetEnvironmentBlueprintByName(const String& name);

	static std::map<String, EnvironmentBlueprint*> s_loadedEnvironmentBlueprints;
};
typedef std::map<String, EnvironmentBlueprint*>::iterator EnvironmentBlueprintMapIterator;