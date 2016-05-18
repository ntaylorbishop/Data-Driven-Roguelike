#include "Game/PlayingGameState.hpp"
#include "Game/LevelSelectState.hpp"
#include "Game/ExitGameState.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
PlayingGameState::PlayingGameState()
	: State("PlayingGameState")
	, m_age(0.f)
	, m_screenImage(nullptr)
	, m_squirrelFont(nullptr)
{
	m_screenImage = Texture::CreateOrGetTexture("Data/Images/MainMenuScreen.png");
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");
}

//---------------------------------------------------------------------------------------------------------------------------
//ENTER UPDATE RENDER EXIT
//---------------------------------------------------------------------------------------------------------------------------
void PlayingGameState::Enter() {

}

State* PlayingGameState::Update(float deltaSeconds) {
	State* state = SwitchStates();
	if (nullptr == state) {
		UpdatePlayingGame(deltaSeconds);
	}

	return state;
}
void PlayingGameState::UpdatePlayingGame(float deltaSeconds) {
	g_theWorld->Update(deltaSeconds);
	g_thePauseMenu->Update(deltaSeconds);
}

void PlayingGameState::Render() const {
	g_theRenderer->ClearScreen(0.f, 0.f, 0.f);

	g_theWorld->Render();
	g_thePauseMenu->Render();
}

bool PlayingGameState::Exit() {
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
State* PlayingGameState::SwitchStates() {
	if (g_theInputSystem->GetKeyDown(VK_ESCAPE)) {
		g_thePauseMenu->Toggle();
	}

	if (g_thePauseMenu->IsExiting()) {
		g_theWorld->FlushActiveEntities();
		g_theUI->SetIsPlayingGame(false);
		//g_theUI->RefreshUI();

		return new LevelSelectState();
	}

	if (g_theWorld->m_exiting) {
		g_theWorld->FlushActiveEntities();
		g_theUI->SetIsPlayingGame(false);

		return new ExitGameState();
	}

	return nullptr;
}