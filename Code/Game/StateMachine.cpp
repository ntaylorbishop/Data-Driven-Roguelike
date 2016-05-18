#include "Game/StateMachine.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS INITIALIZATION
//---------------------------------------------------------------------------------------------------------------------------
StateMachine::StateMachine() 
	: m_currentState(nullptr)
{ }

//---------------------------------------------------------------------------------------------------------------------------
//REGISTER GO TO
//---------------------------------------------------------------------------------------------------------------------------
/*
VIRTUAL void StateMachine::RegisterState(State* state) {

	bool stateExists = false;

	for (unsigned int i = 0; i < m_states.size(); i++) {
		if (m_states[i]->GetName() == state->GetName()) {
			stateExists = true;
			break;
		}
	}

	if (!stateExists) {
		m_states.push_back(state);
	}
}
VIRTUAL void StateMachine::GoToState(State* state) {
	if (m_currentState == state) {
		DebuggerPrintf("ERROR: ALREADY IN STATE ");
		DebuggerPrintf(state->GetName().c_str());
		DebuggerPrintf("\n");
	}
	else {
		m_currentState = state;
	}
}*/

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void StateMachine::Update(float deltaSeconds) {
	State* state = m_currentState->Update(deltaSeconds);

	//DebuggerPrintf(m_currentState->GetName().c_str());

	if (state != NULL) {
		delete m_currentState;
		m_currentState = state;
	}
}