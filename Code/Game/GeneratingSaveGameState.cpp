#include "Game/GeneratingSaveGameState.hpp"
#include "Game/LevelSelectState.hpp"
#include "Game/PlayingGameState.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
GeneratingSaveGameState::GeneratingSaveGameState()
	: State("GeneratingSaveGameState")
	, m_isFinished(false)
{ }

//---------------------------------------------------------------------------------------------------------------------------
//ENTER UPDATE RENDER EXIT
//---------------------------------------------------------------------------------------------------------------------------
void GeneratingSaveGameState::Enter() { }

State* GeneratingSaveGameState::Update(float deltaSeconds) {
	State* state = SwitchStates();
	if (nullptr == state) {
		UpdateGeneratingSaveGameState(deltaSeconds);
	}

	return state;
}
void GeneratingSaveGameState::UpdateGeneratingSaveGameState(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);
	
}

void GeneratingSaveGameState::Render() const {
	g_theRenderer->ClearScreen(0.f, 0.f, 0.f);

	//g_theWorld->Render();
}

bool GeneratingSaveGameState::Exit() {
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
State* GeneratingSaveGameState::SwitchStates() {

		g_theWorld->InitializeEntitiesForLoadedGame();
		g_theSaveLoadManager->LoadGame();
		Generator::FinalizeLoadedMap(g_theWorld->m_activeMap);
		g_theUI->RefreshUI();
		g_theCombatSystem->LoadFromSavedGame();
		g_theMessageLog->LoadFromSavedGame();
		return new PlayingGameState();
}