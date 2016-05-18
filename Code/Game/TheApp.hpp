#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/EventSystem.hpp"
#include "Engine/General/Input/InputSystem.hpp"

class TheApp;
extern TheApp* g_theApp;
extern EventSystem* g_eventSystem;

class TheApp {
public:
	//STRUCTORS
	TheApp(const ScreenCoords& screenDimensions);
	~TheApp();

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	//INPUT
	void HandleInputEvents();

	//FOCUS
	bool HasFocus();
	void OnGainedFocus();
	void OnLostFocus();

	//GETTERS SETTERS
	ScreenCoords GetScreenDimensions() const;
private:
	ScreenCoords m_screenDimensions;
	bool m_windowInFocus;
};