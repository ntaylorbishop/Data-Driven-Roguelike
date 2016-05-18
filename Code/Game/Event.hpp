#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class Event {
public:
	//STRUCTORS
	Event();
	Event(const String& name);

	//GETTERS SETTERS
	String GetName() const;

private:
	String m_name;
};