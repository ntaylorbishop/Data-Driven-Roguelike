#pragma once

#include <deque>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/GameCommon.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//GLOBAL SINGLETON
//---------------------------------------------------------------------------------------------------------------------------
class TheMessageLog;
extern TheMessageLog* g_theMessageLog;

//---------------------------------------------------------------------------------------------------------------------------
//MESSAGE
//---------------------------------------------------------------------------------------------------------------------------
enum eMsgPriority {
	PRIORITY_LOW = 0,
	PRIORITY_MEDIUM,
	PRIORITY_HIGH,
	NUM_PRIORITIES
};
struct Message {
	Message(String message, eMsgPriority priority, uint turnNumber) : m_message(message), m_priority(priority), m_turnNumber(turnNumber) { }

	String m_message;
	eMsgPriority m_priority;
	uint m_turnNumber;
};


class TheMessageLog {
public:
	//STRUCTORS
	TheMessageLog();

	void LoadFromSavedGame();

	//LOGGING
	void LogMessage(const String& str, eMsgPriority priority);

	//UPATE RENDER
	void Update(float deltaSeconds); //SHOULD BE LAST THING CALLED
	void Render() const;
	
	void Enable() { m_isEnabled = true; }
	void Disable() { m_isEnabled = false; }

	//SAVE LOADING
	XMLNode SaveToXML() const;
	static void LoadFromXML(const XMLNode& node);

private:
	std::vector<Message> m_messagesBeingDisplayed;
	std::vector<Message> m_potentialMessages;

	bool m_isEnabled;

	static std::vector<Message>* s_loadedMessages;
};