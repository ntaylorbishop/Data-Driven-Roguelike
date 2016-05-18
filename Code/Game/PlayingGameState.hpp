#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Game/State.hpp"
#include "Game/TheWorld.hpp"
#include "Game/TheUI.hpp"

class PlayingGameState : public State {
public:
	//STRUCTORS
	PlayingGameState();

	//ENTER UPDATE RENDER EXIT
	void Enter();

	State* Update(float deltaSeconds);
	void UpdatePlayingGame(float deltaSeconds);

	void Render() const;

	bool Exit();

private:
	State* SwitchStates();

private:
	//INHERITED:
	//String m_name;

	Texture* m_screenImage;
	float m_age;
	BitmapFont* m_squirrelFont;
};