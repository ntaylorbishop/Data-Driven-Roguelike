#include "Game/TheApp.hpp"

TheApp* g_theApp = nullptr;
EventSystem* g_eventSystem = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
TheApp::TheApp(const ScreenCoords& screenDimensions)
	: m_screenDimensions(screenDimensions)
{ 
	g_eventSystem = new EventSystem();
}
TheApp::~TheApp() {
	delete g_eventSystem;
	g_eventSystem = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheApp::Update(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);
}
void TheApp::Render() const {

}

//---------------------------------------------------------------------------------------------------------------------------
//INPUT
//---------------------------------------------------------------------------------------------------------------------------
void TheApp::HandleInputEvents() {
	if (g_theInputSystem->GetKeyDown(VK_RETURN)) {
		g_eventSystem->RegisterEvent(Event("PressedEnter"));
	}
	if (g_theInputSystem->GetKeyDown(VK_ESCAPE)) {
		g_eventSystem->RegisterEvent(Event("PressedEscape"));
	}
	if (g_theInputSystem->GetKeyDown('1')) {
		g_eventSystem->RegisterEvent(Event("Pressed1"));
	}
	if (g_theInputSystem->GetKeyDown('2')) {
		g_eventSystem->RegisterEvent(Event("Pressed2"));
	}
	if (g_theInputSystem->GetKeyDown('3')) {
		g_eventSystem->RegisterEvent(Event("Pressed3"));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
//FOCUS
//---------------------------------------------------------------------------------------------------------------------------
bool TheApp::HasFocus() {
	return m_windowInFocus;
}
void TheApp::OnGainedFocus() {
	m_windowInFocus = true;
}
void TheApp::OnLostFocus() {
	m_windowInFocus = false;
}

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
ScreenCoords TheApp::GetScreenDimensions() const {
	return m_screenDimensions;
}