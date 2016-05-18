#pragma once

#include "Engine/General/Core/GameEngine.hpp"
#include "Engine/General/Core/WindowHandler.hpp"
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Fonts/Font.hpp"
#include "Game/TheMessageLog.hpp"
#include "Game/TheInfoLog.hpp"
#include "Game/TheInventoryLog.hpp"
#include "Game/TheAnimationManager.hpp"
#include "Game/PauseMenu.hpp"

class TheUI;
extern TheUI* g_theUI;

class TheUI {
public:
	//STRUCTORS
	TheUI();
	~TheUI();

	void RefreshUI();

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	//GETTERS SETTERS
	void SetIsPlayingGame(bool isPlayingGame) { m_isPlayingGame = isPlayingGame; }

	//Drawing
	void DrawTextAtLocation(const Vector2& location, const String& str, float scale, const RGBA& color);

	Font* m_font;
	BitmapFont* m_squirrelFont;

private:
	void RenderDebugText() const;


	std::vector<AABB2> m_testBoxes;
	bool m_isPlayingGame;
};