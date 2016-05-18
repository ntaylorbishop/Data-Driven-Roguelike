#include "Game/TheUI.hpp"
#include "Game/TheCombatSystem.hpp"
#include "Game/TheWorld.hpp"

TheUI* g_theUI = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
TheUI::TheUI() 
	: m_isPlayingGame(false)
{
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");

	m_font = Font::CreateOrGetFont("Tahoma");

	g_theMessageLog = new TheMessageLog();
	g_theInfoLog = new TheInfoLog();
	g_theInventoryLog = new TheInventoryLog();
	g_theAnimationManager = new TheAnimationManager();
	g_thePauseMenu = new PauseMenu();
}
TheUI::~TheUI() {
	delete g_thePauseMenu;
	delete g_theAnimationManager;
	delete g_theInventoryLog;
	delete g_theMessageLog;
	delete g_theInfoLog;
	g_thePauseMenu = nullptr;
	g_theAnimationManager = nullptr;
	g_theInventoryLog = nullptr;
	g_theMessageLog = nullptr;
	g_theInfoLog = nullptr;
}

void TheUI::RefreshUI() {
	delete g_thePauseMenu;
	delete g_theAnimationManager;
	delete g_theInventoryLog;
	delete g_theMessageLog;
	delete g_theInfoLog;
	g_thePauseMenu = nullptr;
	g_theAnimationManager = nullptr;
	g_theInventoryLog = nullptr;
	g_theMessageLog = nullptr;
	g_theInfoLog = nullptr;

	g_theMessageLog = new TheMessageLog();
	g_theInfoLog = new TheInfoLog();
	g_theInventoryLog = new TheInventoryLog();
	g_theAnimationManager = new TheAnimationManager();
	g_thePauseMenu = new PauseMenu();

	////////////////////////////////////////////////////
	//------------------------------------------
	//REFRESH COMBAT SYSTEM
	if (g_theCombatSystem) {
		delete g_theCombatSystem;
		g_theCombatSystem = nullptr;
	}

	g_theCombatSystem = new TheCombatSystem(g_theWorld->m_entitiesOnMap);

	m_isPlayingGame = true;
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheUI::Update(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

	if (m_isPlayingGame) {
		g_theInventoryLog->Update(deltaSeconds);
		g_theMessageLog->Update(deltaSeconds);
		g_theInfoLog->Update(deltaSeconds);
		g_theAnimationManager->Update(deltaSeconds);
	}
}
void TheUI::Render() const {
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(1600.f, 900.f));

	if (m_isPlayingGame) {
		g_theInventoryLog->Render();
		g_theMessageLog->Render();
		g_theInfoLog->Render();
		g_theAnimationManager->Render();
	}
	
	RenderDebugText();
}

//---------------------------------------------------------------------------------------------------------------------------
//DRAWING
//---------------------------------------------------------------------------------------------------------------------------
void TheUI::DrawTextAtLocation(const Vector2& location, const String& str, float scale, const RGBA& color) {
	m_font->DrawText2D(location, str, scale, color);
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE
//---------------------------------------------------------------------------------------------------------------------------
void TheUI::RenderDebugText() const {		
/*
	String sampleText = StringUtils::Stringf("THIS IS SAMPLE TEXT");

	IntVector2 relMousePos = g_theWindowHandler->GetMainWindow().GetRelativeMousePosition();
	String mousePosStr = "X: " + to_string(relMousePos.x) + " Y: " + to_string(relMousePos.y);

	m_font->DrawText2D(Vector2(10.f, 850.f), mousePosStr, 0.4f, RGBA::YELLOW);*/
}