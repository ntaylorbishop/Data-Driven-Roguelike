#include "Game/ExitGameState.hpp"
#include "Game/MainMenuState.hpp"
#include "Game/TheGame.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
ExitGameState::ExitGameState()
	: State("ExitGameState")
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
void ExitGameState::Enter() {

}

State* ExitGameState::Update(float deltaSeconds) {
	State* state = SwitchStates();
	if (nullptr == state) {
		UpdatePlayingGame(deltaSeconds);
	}

	return state;
}
void ExitGameState::UpdatePlayingGame(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);
}

void ExitGameState::Render() const {
	g_theRenderer->ClearScreen(0.f, 0.f, 0.f);

	String monstersKilled = StringUtils::Stringf("Monsters Killed: %i", g_theGame->m_numMonstersKilled);
	String turnCount = StringUtils::Stringf("Turn Count:      %i", g_theGame->m_turnCountPlayerDied);

	m_squirrelFont->DrawText2D(Vector2(115.f, 750.f), "YOU HAVE DIED", 100.f, RGBA::WHITE, nullptr);

	m_squirrelFont->DrawText2D(Vector2(100.f, 500.f), monstersKilled, 50.f, RGBA::WHITE, nullptr);
	m_squirrelFont->DrawText2D(Vector2(100.f, 450.f), turnCount, 50.f, RGBA::WHITE, nullptr);

}

bool ExitGameState::Exit() {
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
State* ExitGameState::SwitchStates() {
	if (g_theInputSystem->GetKeyDown(VK_ESCAPE) || g_theInputSystem->GetKeyDown(VK_RETURN)) {
		g_theWorld->FlushActiveEntities();
		g_theUI->RefreshUI();
		g_theUI->SetIsPlayingGame(false);
		return new MainMenuState();
	}

	return nullptr;
}