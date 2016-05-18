#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/ErrorWarningAssert.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/Map.hpp"
#include "Game/EnvironmentGeneratorData.hpp"
#include "Game/TheCombatSystem.hpp"
#include "Game/ItemFactory.hpp"
#include "Game/FeatureFactory.hpp"


//---------------------------------------------------------------------------------------------------------------------------
//GENERATOR
//---------------------------------------------------------------------------------------------------------------------------
class Generator {
public:
	Generator(EnvironmentGeneratorData* genData);

	virtual Map* GenerateMap(const IntVector2& size);
	virtual bool GenerateStep(Map* map, int& currentStepNumber) = 0;
	virtual void GenerateAll(Map* map, int numSteps) = 0;


	static void FinalizeMap(Map* map);
	static void FinalizeLoadedMap(Map* map);
	static void PopulateMap(Map* map);

protected:
	EnvironmentGeneratorData* m_generatorData;
};

typedef Generator* (GeneratorCreationFunc)(EnvironmentGeneratorData* genData);

//---------------------------------------------------------------------------------------------------------------------------
//GENERATOR REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
class GeneratorRegistration {
public:
	GeneratorRegistration(const String& name, GeneratorCreationFunc* creationFunc);
	static Generator* CreateGeneratorByName(const String& name, EnvironmentGeneratorData* genData);

	static std::map<String, GeneratorRegistration*>* GeneratorRegistration::s_generatorRegistrationMap;

private:
	String m_name;
	GeneratorCreationFunc* m_creationFunc;

};