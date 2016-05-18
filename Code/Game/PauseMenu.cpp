#include "Game/PauseMenu.hpp"
#include "Game/TheUI.hpp"
#include "Game/TheSaveLoadManager.hpp"
#include "Engine/General/Input/InputSystem.hpp"

PauseMenu* g_thePauseMenu = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
PauseMenu::PauseMenu() 
	: m_isActive(false)
	, m_isExiting(false) 
{
	AABB2Int resumeBtn = AABB2Int(IntVector2(550, 450), IntVector2(1050, 500));
	AABB2Int saveBtn = AABB2Int(IntVector2(550, 350), IntVector2(1050, 400));
	AABB2Int exitBtn = AABB2Int(IntVector2(550, 250), IntVector2(1050, 300));

	m_resumeBtn = UIButton(resumeBtn.mins.x, resumeBtn.maxs.x, resumeBtn.mins.y, resumeBtn.maxs.y, RGBA::WHITE, RGBA::WHITE);
	m_saveBtn = UIButton(saveBtn.mins.x, saveBtn.maxs.x, saveBtn.mins.y, saveBtn.maxs.y, RGBA::WHITE, RGBA::WHITE);
	m_exitBtn = UIButton(exitBtn.mins.x, exitBtn.maxs.x, exitBtn.mins.y, exitBtn.maxs.y, RGBA::WHITE, RGBA::WHITE);
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void PauseMenu::Update(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

	m_isExiting = false;

	if (m_isActive) {
		if (m_resumeBtn.HasBeenClickedOnThisFrame()) {
			m_isActive = false;
		}
		else if (m_saveBtn.HasBeenClickedOnThisFrame()) {
			//todo
			g_theSaveLoadManager->SaveGame();
		}
		else if (m_exitBtn.HasBeenClickedOnThisFrame()) {
			m_isExiting = true;
			g_theInfoLog->ClearAnimation();
		}
	}
}
void PauseMenu::Render() {
	if (m_isActive) {
		g_theRenderer->DrawSquare(AABB2(Vector2(500.f, 200.f), Vector2(1100, 700.f)), RGBA::BLACK, 4.f);
		g_theRenderer->DrawAABB2(AABB2(Vector2(500.f, 200.f), Vector2(1100, 700.f)), RGBA::GREY, PRIMITIVE_QUADS);

		//BUTTONS
		IntVector2 relMousePos = g_theInputSystem->GetRelativeCursorPosition();

		////////////////////////////////////////////////////
		//------------------------------------------
		//RESUME
		Vector2 botLeft = Vector2((float)m_resumeBtn.left, (float)m_resumeBtn.bottom);
		Vector2 topRight = Vector2((float)m_resumeBtn.right, (float)m_resumeBtn.top);
		AABB2 box = AABB2(botLeft, topRight);
		RGBA color = RGBA::WHITE;

		color = (m_resumeBtn.IsPositionInRect(relMousePos)) ? (RGBA(1.f, 1.f, 0.f, 0.6f)) : (RGBA(0.f, 0.f, 0.f, 1.f));
		g_theRenderer->DrawAABB2(box, color, PRIMITIVE_QUADS);

		Vector2 textDrawLoc;
		float widthOfString = g_theUI->m_font->GetWidthOfString("RESUME", 0.8f);
		textDrawLoc.x = (((m_resumeBtn.right - m_resumeBtn.left) / 2.f) - widthOfString / 2.f) + m_resumeBtn.left;
		textDrawLoc.y = m_resumeBtn.bottom + 5.f;
		g_theUI->DrawTextAtLocation(textDrawLoc, "RESUME", 0.8f, RGBA::WHITE);

		////////////////////////////////////////////////////
		//------------------------------------------
		//SAVE
		botLeft = Vector2((float)m_saveBtn.left, (float)m_saveBtn.bottom);
		topRight = Vector2((float)m_saveBtn.right, (float)m_saveBtn.top);
		box = AABB2(botLeft, topRight);
		color = RGBA::WHITE;

		color = (m_saveBtn.IsPositionInRect(relMousePos)) ? (RGBA(1.f, 1.f, 0.f, 0.6f)) : (RGBA(0.f, 0.f, 0.f, 1.f));
		g_theRenderer->DrawAABB2(box, color, PRIMITIVE_QUADS);

		widthOfString = g_theUI->m_font->GetWidthOfString("SAVE", 0.8f);
		textDrawLoc.x = (((m_saveBtn.right - m_saveBtn.left) / 2.f) - widthOfString / 2.f) + m_saveBtn.left;
		textDrawLoc.y = m_saveBtn.bottom + 5.f;
		g_theUI->DrawTextAtLocation(textDrawLoc, "SAVE", 0.8f, RGBA::WHITE);

		////////////////////////////////////////////////////
		//------------------------------------------
		//EXIT
		botLeft = Vector2((float)m_exitBtn.left, (float)m_exitBtn.bottom);
		topRight = Vector2((float)m_exitBtn.right, (float)m_exitBtn.top);
		box = AABB2(botLeft, topRight);
		color = RGBA::WHITE;

		color = (m_exitBtn.IsPositionInRect(relMousePos)) ? (RGBA(1.f, 1.f, 0.f, 0.6f)) : (RGBA(0.f, 0.f, 0.f, 1.f));
		g_theRenderer->DrawAABB2(box, color, PRIMITIVE_QUADS);

		widthOfString = g_theUI->m_font->GetWidthOfString("EXIT", 0.8f);
		textDrawLoc.x = (((m_exitBtn.right - m_exitBtn.left) / 2.f) - widthOfString / 2.f) + m_exitBtn.left;
		textDrawLoc.y = m_exitBtn.bottom + 5.f;
		g_theUI->DrawTextAtLocation(textDrawLoc, "EXIT", 0.8f, RGBA::WHITE);


		g_theUI->DrawTextAtLocation(Vector2(715.f, 600.f), "PAUSED", 1.f, RGBA::BLACK);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
void PauseMenu::Toggle() {
	m_isActive = !m_isActive;

	if (!m_isActive) {
		g_theInfoLog->ClearAnimation();
	}
}