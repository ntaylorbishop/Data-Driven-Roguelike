#include "Game/FeatureFactory.hpp"

STATIC std::map<String, FeatureFactory*> FeatureFactory::s_featureFactories;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
FeatureFactory::FeatureFactory(const XMLNode& featureBlueprintNode) {
	PopulateFromXMLNode(featureBlueprintNode);
}
void FeatureFactory::PopulateFromXMLNode(const XMLNode& featureBlueprintNode) {

	m_templateFeature = new Feature(featureBlueprintNode);
	m_name = m_templateFeature->GetName();
}

//---------------------------------------------------------------------------------------------------------------------------
//CREATION
//---------------------------------------------------------------------------------------------------------------------------
Feature* FeatureFactory::Create(Map* belongingMap, const IntVector2& loc) {

	Feature* newFeature = new Feature(m_templateFeature);

	newFeature->SetFeatureState(FEATURE_STATE_DEACTIVATED);
	newFeature->SetPosition(loc);
	newFeature->SetBelongingMap(belongingMap);

	switch (newFeature->GetFeatureType()) {
	case FEATURE_TYPE_DOOR:
		newFeature->SetFeatureState(FEATURE_STATE_ACTIVATED);
		break;
	default:
		break;
	}

	return newFeature;
}
STATIC FeatureFactory* FeatureFactory::GetRandomFactory() {
	std::map<String, FeatureFactory*>::iterator item = s_featureFactories.begin();
	std::advance(item, RandInt(0, s_featureFactories.size() - 1));
	return item->second;
}
STATIC FeatureFactory* FeatureFactory::GetRandomFactoryBasedOnType(eFeatureType featureType) {
	UNREFERENCED_PARAMETER(featureType);
/*
	std::vector<FeatureFactory*> factoriesOfType;

/ *
	for (std::map<String, FeatureFactory*>::iterator item = s_featureFactories.begin(); item != s_featureFactories.end(); item++) {
		
		if (item->second->GetFeatureType() == featureType) {
			factoriesOfType.push_back(item->second);
		}

		eFeatureType type = item->second->GetFeatureType();
			int a = 0;
			 
	}* /

	int which = RandIntZeroToSize(factoriesOfType.size());
	*/

	return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
STATIC void FeatureFactory::LoadAllFeatures() {
	String relDir = "Data/Features/";
	std::vector<String> filePaths = FileUtils::GetAllFilenamesInDirectory(relDir);

	for (unsigned int i = 0; i < filePaths.size(); i++) {
		XMLNode root = XMLNode::parseFile(filePaths[i].c_str());
		root = root.getChildNode(0);

		for (int j = 0; j < root.nChildNode(); j++) {
			XMLNode npcBlueprint = root.getChildNode(j);

			FeatureFactory* currFactory = new FeatureFactory(npcBlueprint);


			s_featureFactories.insert(std::pair<String, FeatureFactory*>(currFactory->GetName(), currFactory));
		}
	}
}
STATIC FeatureFactory* FeatureFactory::CreateByName(const String& name) {
	std::map<String, FeatureFactory*>::iterator factory = s_featureFactories.find(name);

	if (factory == s_featureFactories.end()) {
		DebuggerPrintf("%s%s", "Couldn't find Feature blueprint ", name);
		return nullptr;
	}
	else {
		return factory->second;
	}
}