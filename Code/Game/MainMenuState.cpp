#include "Game/MainMenuState.hpp"
#include "Game/LevelSelectState.hpp"
#include "Engine/General/Core/GameEngine.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
MainMenuState::MainMenuState()
	: State("MainMenu")
	, m_shouldGenerateAllAtOnce(false)
{ 
	m_screenImage = Texture::CreateOrGetTexture("Data/Images/MainMenuScreen.png");
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");

	m_age = 0.f;
}

//---------------------------------------------------------------------------------------------------------------------------
//ENTER UPDATE RENDER EXIT
//---------------------------------------------------------------------------------------------------------------------------
void MainMenuState::Enter() {

}

State* MainMenuState::Update(float deltaSeconds) {
	State* state = SwitchStates();
	if (nullptr == state) {
		UpdateMainMenu(deltaSeconds);
	}

	return state;
}
void MainMenuState::UpdateMainMenu(float deltaSeconds) {
	m_age += deltaSeconds;

	if (m_tabSwitchAge >= 0.2f) {
		m_tabSwitchAge -= deltaSeconds;
	}

	if (g_theInputSystem->GetKeyDown(VK_TAB)) {
		m_shouldGenerateAllAtOnce = !m_shouldGenerateAllAtOnce;
	}
}

void MainMenuState::Render() const {
	g_theRenderer->ClearScreen(0.f, 0.f, 0.f);

	AABB2 quad = AABB2(Vector2(0.f, 0.f), Vector2(1600.f, 900.f));
	g_theRenderer->DrawTexturedAABB2(m_screenImage, RGBA::WHITE, quad);

	float frequency = 1; // Frequency in Hz
	float opacity =  0.5f * (1 + sin(2.f * pi * frequency * m_age));

	//float opacity = 0.5f * sin(m_age * 4.f) + 1.f;
	//opacity = ClampFloat(opacity, 0.f, 1.f);

	String enterText = StringUtils::Stringf("PRESS ENTER");

	m_squirrelFont->DrawText2D(Vector2(470.f, 100.f), enterText, 60.f, RGBA(1.f, 1.f, 1.f, opacity), NULL);
}

bool MainMenuState::Exit() {
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
State* MainMenuState::SwitchStates() {
	if (g_theInputSystem->GetKeyDown(VK_RETURN)) {
		return new LevelSelectState();
	}
	else if (g_theInputSystem->GetKeyDown(VK_ESCAPE)) {
		if (!g_theApparatusEngine->IsConsoleOpen()) {
			g_theInputSystem->Quitting(true);
		}
	}

	return nullptr;
}