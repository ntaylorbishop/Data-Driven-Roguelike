#pragma once

#include "Game/StateMachine.hpp"
#include "Game/State.hpp"
#include "Game/MainMenuState.hpp"
#include "Game/LevelSelectState.hpp"
#include "Game/OptionsMenuState.hpp"
#include "Game/PlayingGameState.hpp"


class MenuStateMachine : public StateMachine {
public:
	//STRUCTORS INITIALIZATION
	MenuStateMachine();

	void Render() const;

private:
	//INHERITED:
	//State* m_currentState;
};