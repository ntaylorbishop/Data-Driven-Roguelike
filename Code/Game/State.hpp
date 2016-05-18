#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Console/Command.hpp"

class State {
public:
	//STRUCTORS
	State();
	State(String name);

	//ENTER EXIT UPDATE RENDER
	virtual void Enter() = 0;

	virtual State* Update(float deltaSeconds) = 0;
	virtual void Render() const = 0;

	virtual bool Exit() = 0;

	//GETTERS SETTERS
	String GetName() const;

protected:
	String m_name;
};
typedef std::map<String, State*>::iterator StateMapIterator;

//---------------------------------------------------------------------------------------------------------------------------
//OPERATOR OVERLOADS
//---------------------------------------------------------------------------------------------------------------------------
bool operator==(const State& lhs, const State& rhs);
bool operator!=(const State& lhs, const State& rhs);