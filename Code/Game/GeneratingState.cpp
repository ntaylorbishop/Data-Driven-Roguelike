#include "Game/GeneratingState.hpp"
#include "Game/LevelSelectState.hpp"
#include "Game/PlayingGameState.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
GeneratingState::GeneratingState(const String& environmentName, bool generateAllAtOnce)
	: State("LevelSelectState")
	, m_hasFinishedGenerating(false)
	, m_generateAllAtOnce(generateAllAtOnce)
	, m_activeEnvironmentBlueprint(nullptr)
	, m_currGenerator(nullptr)
	, m_currStepNumber(0)
	, m_currGenData(nullptr)
	, m_currGenDataIdx(0)
{ 
	m_activeEnvironmentBlueprint = EnvironmentBlueprint::GetEnvironmentBlueprintByName(environmentName);

	String currGeneratorName = m_activeEnvironmentBlueprint->m_genDatas[0]->GetGeneratorName();
	m_currGenData = m_activeEnvironmentBlueprint->m_genDatas[0];
	m_currGenerator = GeneratorRegistration::CreateGeneratorByName(currGeneratorName, m_currGenData);

	Map* activeMap = m_currGenerator->GenerateMap(m_activeEnvironmentBlueprint->GetSize());
	g_theWorld->m_activeMap = activeMap;
}

//---------------------------------------------------------------------------------------------------------------------------
//ENTER UPDATE RENDER EXIT
//---------------------------------------------------------------------------------------------------------------------------
void GeneratingState::Enter() {

}

State* GeneratingState::Update(float deltaSeconds) {
	State* state = SwitchStates();
	if (nullptr == state) {
		UpdateGeneratingState(deltaSeconds);
	}

	return state;
}
void GeneratingState::UpdateGeneratingState(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);


	if (m_generateAllAtOnce) {
		GenerateAllAtOnce();
	}

	if (g_theInputSystem->GetKeyDown(VK_SPACE)) {
		if (m_currStepNumber > m_currGenData->GetGeneratorNumSteps()) {
			if (m_currGenDataIdx == (int)m_activeEnvironmentBlueprint->m_genDatas.size() - 1) {
				m_hasFinishedGenerating = true;
				return;
			}
			else {
				m_currGenDataIdx++;
				m_currGenData = m_activeEnvironmentBlueprint->m_genDatas[m_currGenDataIdx];
				m_currStepNumber = 0;

				delete m_currGenerator;
				String generatorName = m_currGenData->GetGeneratorName();
				m_currGenerator = GeneratorRegistration::CreateGeneratorByName(generatorName, m_currGenData);
			}
		}

		m_currGenerator->GenerateStep(g_theWorld->m_activeMap, m_currStepNumber);
		m_currStepNumber++;
	}
}

void GeneratingState::GenerateAllAtOnce() {
	for (unsigned int i = 0; i < m_activeEnvironmentBlueprint->m_genDatas.size(); i++) {
		int maxSteps = m_activeEnvironmentBlueprint->m_genDatas[i]->GetGeneratorNumSteps();

		if (i != 0) {
			String generatorName = m_activeEnvironmentBlueprint->m_genDatas[i]->GetGeneratorName();
			m_currGenerator = GeneratorRegistration::CreateGeneratorByName(generatorName, m_activeEnvironmentBlueprint->m_genDatas[i]);
		}

		for (int j = 0; j < maxSteps; j++) {
			m_currGenerator->GenerateStep(g_theWorld->m_activeMap, j);
		}
	}

	m_hasFinishedGenerating = true;
}

void GeneratingState::Render() const {
	g_theRenderer->ClearScreen(0.f, 0.f, 0.f);

	g_theWorld->Render();
}

bool GeneratingState::Exit() {
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
State* GeneratingState::SwitchStates() {
	if (g_theInputSystem->GetKeyDown(VK_ESCAPE)) {
		return new LevelSelectState();
	}
	if (m_hasFinishedGenerating) {
		g_theWorld->InitializeEntities();
		//m_currGenerator->FinalizeMap(g_theWorld->m_activeMap);
		Generator::FinalizeMap(g_theWorld->m_activeMap);

		g_theUI->RefreshUI();
		return new PlayingGameState();
	}

	return nullptr;
}