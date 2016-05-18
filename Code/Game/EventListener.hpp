#pragma once

#include <string>

#include "Engine/General/Core/EngineCommon.hpp"

class EventListener {
public:
	EventListener();

	bool HasEventFired() const;
	void SetHasEventFired(bool hasFired);

	String GetEventListeningFor() const;
	String GetName() const;

private:
	bool m_hasEventFired;
	String m_eventListeningFor;
	String m_name;
};