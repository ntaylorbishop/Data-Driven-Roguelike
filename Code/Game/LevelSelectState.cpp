#include "Game/LevelSelectState.hpp"
#include "Game/MainMenuState.hpp"
#include "Game/PlayingGameState.hpp"
#include "Game/GeneratingState.hpp"
#include "Game/GeneratingSaveGameState.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
LevelSelectState::LevelSelectState() 
	: State("LevelSelectState")
	, m_age(0.f)
	, m_screenImage(nullptr)
	, m_squirrelFont(nullptr)
	, m_shouldGenerateAllAtOnce(false)
	, m_tabSwitchAge(0.2f)
{ 
	m_screenImage = Texture::CreateOrGetTexture("Data/Images/MainMenuScreen.png");
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");

	std::map<String, EnvironmentBlueprint*>::iterator mapIt;
	for (mapIt = EnvironmentBlueprint::s_loadedEnvironmentBlueprints.begin(); mapIt != EnvironmentBlueprint::s_loadedEnvironmentBlueprints.end(); mapIt++) {
		EnvironmentBlueprint* currBlueprint = mapIt->second;
		m_environments.push_back(currBlueprint->GetName());
	}

	int offset = 36;
	IntVector2 start = IntVector2(200, 210 + offset);
	for (uint i = 0; i < m_environments.size(); i++) {
		uint currTop = (210 + offset) - (offset * i);
		m_rects.push_back(UIButton(start.x, start.x + 1200, currTop - offset + 1, currTop, RGBA(1.f, 1.f, 1.f, 0.5f), RGBA::BLACK));
	}

	if (g_theSaveLoadManager->DoesSaveGameExist()) {
		uint currTop = (210 + offset) - (offset * m_environments.size());
		m_rects.push_back(UIButton(start.x, start.x + 1200, currTop - offset + 1, currTop, RGBA(1.f, 1.f, 1.f, 0.5f), RGBA::BLACK));
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//ENTER UPDATE RENDER EXIT
//---------------------------------------------------------------------------------------------------------------------------
void LevelSelectState::Enter() {

}

State* LevelSelectState::Update(float deltaSeconds) {
	State* state = SwitchStates();
	if (nullptr == state) {
		UpdateLevelSelect(deltaSeconds);
	}

	return state;
}
void LevelSelectState::UpdateLevelSelect(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

	if (m_tabSwitchAge >= 0.2f) {
		m_tabSwitchAge -= deltaSeconds;
	}

	if (g_theInputSystem->GetKeyDown(VK_TAB)) {
		m_shouldGenerateAllAtOnce = !m_shouldGenerateAllAtOnce;
		m_tabSwitchAge = 1.f;
	}
}

void LevelSelectState::Render() const {
	g_theRenderer->ClearScreen(0.f, 0.f, 0.f);

	AABB2 quad = AABB2(Vector2(0.f, 0.f), Vector2(1600.f, 900.f));
	g_theRenderer->DrawTexturedAABB2(m_screenImage, RGBA::WHITE, quad);

	int currLvlNum = 1;
	Vector2 textLocation = Vector2(300.f, 210.f);
	for (unsigned int envIdx = 0; envIdx < m_environments.size(); envIdx++) {

		String text = StringUtils::Stringf("%i) %s", currLvlNum, m_environments[envIdx].c_str());
		m_squirrelFont->DrawText2D(textLocation, text, 30.f, RGBA::WHITE, NULL);

		textLocation = textLocation + Vector2(0.f, -35.f);
		currLvlNum++;
	}

	if (g_theSaveLoadManager->DoesSaveGameExist()) {
		String text = StringUtils::Stringf("%i) %s", currLvlNum, "Load Saved Game");
		m_squirrelFont->DrawText2D(textLocation, text, 30.f, RGBA::WHITE, NULL);
	}

	if (m_shouldGenerateAllAtOnce) {
		m_squirrelFont->DrawText2D(Vector2(1300.f, 25.f), "AUTOMATIC MODE", 13.f, RGBA(1.f, 1.f, 1.f, m_tabSwitchAge), NULL);
	}
	else {
		m_squirrelFont->DrawText2D(Vector2(1300.f, 25.f), "NON-AUTOMATIC MODE", 13.f, RGBA(1.f, 1.f, 1.f, m_tabSwitchAge), NULL);
	}

	//MOUSE
	for (unsigned int i = 0; i < m_rects.size(); i++) {
		Vector2 botLeft = Vector2((float)m_rects[i].left, (float)m_rects[i].bottom);
		Vector2 topRight = Vector2((float)m_rects[i].right, (float)m_rects[i].top);
		AABB2 box = AABB2(botLeft, topRight);
		RGBA color = RGBA::WHITE;

		if (m_rects[i].IsPositionInRect(g_theInputSystem->GetRelativeCursorPosition())) {
			color = RGBA(1.f, 1.f, 1.f, 0.6f);
		}
		else {
			color = RGBA(1.f, 1.f, 1.f, 0.2f);
		}
		
		g_theRenderer->DrawAABB2(box, color, PRIMITIVE_QUADS);
	}
}

bool LevelSelectState::Exit() {
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
State* LevelSelectState::SwitchStates() {
	//KEYBOARD
	for (unsigned int i = 0; i < m_environments.size(); i++) {
		char envNum[2];
		_itoa_s(i + 1, envNum, 10);

		if (g_theInputSystem->GetKeyDown(envNum[0])) {
			return new GeneratingState(m_environments[i], m_shouldGenerateAllAtOnce);
		}
	}

	if (g_theSaveLoadManager->DoesSaveGameExist()) {
		char envNum[2];
		_itoa_s(m_environments.size() + 1, envNum, 10);

		if (g_theInputSystem->GetKeyDown(envNum[0])) {
			return new GeneratingSaveGameState();
		}
	}

	//MOUSE
	for (uint i = 0; i < m_rects.size(); i++) {
		if (m_rects[i].HasBeenClickedOnThisFrame()) {
			if (i == m_environments.size()) {
				return new GeneratingSaveGameState();
			}
			return new GeneratingState(m_environments[i], m_shouldGenerateAllAtOnce);
		}
	}

	if (g_theInputSystem->GetKeyDown(VK_ESCAPE)) {
		return new MainMenuState();
	}

	return nullptr;
}