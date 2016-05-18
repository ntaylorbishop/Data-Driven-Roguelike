#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Game/State.hpp"

class StateMachine {
public:
	//STRUCTORS INITIALIZATION
	StateMachine();

	//UPDATE
	void Update(float deltaSeconds);

protected:
	State* m_currentState;
};