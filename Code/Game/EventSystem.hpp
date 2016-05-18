#pragma once

#include <queue>
#include <map>
#include <string>

#include "Game/Event.hpp"
#include "Game/EventListener.hpp"

class EventSystem {
public:
	EventSystem();

	virtual void Update(float deltaSeconds);

	virtual void RegisterEvent(const Event& eventToRegister);
	virtual void RegisterEventListener(const EventListener& eventListener);

private:
	std::queue<Event> m_events;
	std::vector<EventListener> m_eventListeners;
};