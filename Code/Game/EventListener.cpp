#include "Game/EventListener.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
EventListener::EventListener() 
	: m_hasEventFired(false)
{ }

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
bool EventListener::HasEventFired() const {
	return m_hasEventFired;
}
void EventListener::SetHasEventFired(bool hasFired) {
	m_hasEventFired = hasFired;
}

String EventListener::GetEventListeningFor() const {
	return m_eventListeningFor;
}
String EventListener::GetName() const {
	return m_name;
}