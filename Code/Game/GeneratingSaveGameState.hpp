#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Game/State.hpp"
#include "Game/TheWorld.hpp"

class GeneratingSaveGameState : public State {
public:
	//STRUCTORS
	GeneratingSaveGameState();

	//ENTER UPDATE RENDER EXIT
	void Enter();

	State* Update(float deltaSeconds);
	void UpdateGeneratingSaveGameState(float deltaSeconds);

	void Render() const;

	bool Exit();

private:
	State* SwitchStates();

private:
	//INHERITED:
	//String m_name;
	bool m_isFinished;
};