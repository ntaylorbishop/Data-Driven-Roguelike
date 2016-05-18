#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Engine/General/UI/UIButton.hpp"

class PauseMenu;
extern PauseMenu* g_thePauseMenu;

class PauseMenu {
public:
	PauseMenu();

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render();

	//GETTERS SETTERS
	void Toggle();
	bool IsActive() const { return m_isActive; }
	bool IsExiting() const { return m_isExiting; }

private:

	bool m_isActive;
	bool m_isExiting;

	//MOUSE INPUT
	UIButton m_resumeBtn;
	UIButton m_saveBtn;
	UIButton m_exitBtn;
};