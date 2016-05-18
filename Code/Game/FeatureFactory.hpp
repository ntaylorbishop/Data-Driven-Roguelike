#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Feature.hpp"

class FeatureFactory {
public:
	//STRUCTORS
	FeatureFactory(const XMLNode& featureBlueprintNode);
	void PopulateFromXMLNode(const XMLNode& featureBlueprintNode);

	//CREATION
	Feature* Create(Map* belongingMap, const IntVector2& loc);
	static FeatureFactory* GetRandomFactory();
	static FeatureFactory* GetRandomFactoryBasedOnType(eFeatureType featureType);

	//GETTERS SETTERS
	eFeatureType GetFeatureType() const { m_templateFeature->GetFeatureType(); }
	String GetName() const { return m_name; }
	static int GetNumFactories() { return s_featureFactories.size(); }

	//STATIC
	static void LoadAllFeatures();
	static FeatureFactory* CreateByName(const String& name);

private:
	Feature* m_templateFeature;
	String m_name;

	static std::map<String, FeatureFactory*> s_featureFactories;
};