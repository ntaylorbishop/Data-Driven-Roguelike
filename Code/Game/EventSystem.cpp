#include "Game/EventSystem.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
EventSystem::EventSystem() {

}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void EventSystem::Update(float deltaSeconds) {
	deltaSeconds = deltaSeconds;

	while (!m_events.empty()) {
		Event* currEvent = &m_events.front();


		for (unsigned int i = 0; i < m_eventListeners.size(); i++) {
			if (m_eventListeners[i].GetEventListeningFor() == currEvent->GetName()) {
				m_eventListeners[i].SetHasEventFired(true);
			}
		}

		m_events.pop();
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//EVENT REGISTRATION
//---------------------------------------------------------------------------------------------------------------------------
void EventSystem::RegisterEvent(const Event& eventToRegister) {
	m_events.push(eventToRegister);
}
void EventSystem::RegisterEventListener(const EventListener& eventListener) {
	m_eventListeners.push_back(eventListener);
}