#include "Game/TheMessageLog.hpp"
#include "Game/TheUI.hpp"
#include "Game/TheCombatSystem.hpp"

TheMessageLog* g_theMessageLog = nullptr;
STATIC std::vector<Message>* TheMessageLog::s_loadedMessages = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
TheMessageLog::TheMessageLog()
	: m_isEnabled(false)
{ }

void TheMessageLog::LoadFromSavedGame() {
	ASSERT_OR_DIE(s_loadedMessages != nullptr, "ERROR: LOADEDMESSAGES IS NULL");

	for (unsigned int i = 0; i < s_loadedMessages->size(); i++) {
		m_messagesBeingDisplayed.push_back(s_loadedMessages->at(i));
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//LOGGING
//---------------------------------------------------------------------------------------------------------------------------
void TheMessageLog::LogMessage(const String& str, eMsgPriority priority) {
	Message msg = Message(str, priority, g_theCombatSystem->GetTurnCount());

	m_potentialMessages.push_back(msg);
}

//---------------------------------------------------------------------------------------------------------------------------
//UPATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheMessageLog::Update(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

	if (m_potentialMessages.size() == 0) {
		return;
	}

	//HIGHEST PRIORITY
	uint numMsgs = 0;
	for (Message& currMsg : m_potentialMessages) {
		if (currMsg.m_priority == PRIORITY_HIGH) {
			m_messagesBeingDisplayed.push_back(currMsg);
			numMsgs++;

			if (numMsgs == MAX_NEW_MESSAGES_MESSAGE_LOG) {
				break;
			}
		}
	}

	//MEDIUM PRIORITY
	for (Message& currMsg : m_potentialMessages) {
		if (currMsg.m_priority == PRIORITY_MEDIUM) {
			m_messagesBeingDisplayed.push_back(currMsg);
			numMsgs++;

			if (numMsgs == MAX_NEW_MESSAGES_MESSAGE_LOG) {
				break;
			}
		}
	}

	//LOW PRIORITY
	for (Message& currMsg : m_potentialMessages) {
		if (currMsg.m_priority == PRIORITY_LOW) {
			m_messagesBeingDisplayed.push_back(currMsg);
			numMsgs++;

			if (numMsgs == MAX_NEW_MESSAGES_MESSAGE_LOG) {
				break;
			}
		}
	}

	m_potentialMessages.clear();
}
void TheMessageLog::Render() const {

	Vector2 startQuad = MESSAGE_LOG_START_LOC;
	Vector2 endQuad = startQuad + Vector2(800.f, 140.f);

	g_theRenderer->DrawAABB2(AABB2(startQuad, endQuad), RGBA(0.f, 0.f, 0.f, 0.8f), PRIMITIVE_QUADS);


	uint numMsgsBeingDisplayed = 0;

	(m_messagesBeingDisplayed.size() <= MAX_MESSAGES_MESSAGE_LOG) 
		? (numMsgsBeingDisplayed = m_messagesBeingDisplayed.size())
		: (numMsgsBeingDisplayed = MAX_MESSAGES_MESSAGE_LOG);

	Vector2 displayLoc = MESSAGE_LOG_START_LOC + Vector2(0.f, (float)numMsgsBeingDisplayed * 20.f);

	int msgsDispSoFar = 0;
	std::vector<Message>::const_reverse_iterator msgIter;
	for (msgIter = m_messagesBeingDisplayed.rbegin(); msgIter != m_messagesBeingDisplayed.rend(); msgIter++) {

		if (msgsDispSoFar >= MAX_MESSAGES_MESSAGE_LOG) {
			break;
		}

		if (abs((int)msgIter->m_turnNumber - (int)g_theCombatSystem->GetTurnCount()) < 3) {
			g_theUI->DrawTextAtLocation(displayLoc, msgIter->m_message, 0.4f, RGBA::WHITE);
		}
		else {
			g_theUI->DrawTextAtLocation(displayLoc, msgIter->m_message, 0.4f, RGBA(0.7f, 0.7f, 0.7f, 1.f));
		}

		displayLoc += Vector2(0.f, -20.f);

		msgsDispSoFar++;
	}
}

//SAVE LOADING
XMLNode TheMessageLog::SaveToXML() const {
	XMLNode msgNode = msgNode.createXMLTopNode("MessageLog");

	for (unsigned int i = 0; i < m_messagesBeingDisplayed.size(); i++) {
		XMLNode currMsgNode = currMsgNode.createXMLTopNode("Message");
		currMsgNode.addAttribute("text", m_messagesBeingDisplayed[i].m_message.c_str());
		currMsgNode.addAttribute("turnNumber", std::to_string(m_messagesBeingDisplayed[i].m_turnNumber).c_str());
		msgNode.addChild(currMsgNode);
	}

	return msgNode;
}
STATIC void TheMessageLog::LoadFromXML(const XMLNode& node) {
	if (s_loadedMessages) {
		delete s_loadedMessages;
	}
	
	s_loadedMessages = new std::vector<Message>();
	
	for (int i = 0; i < node.nChildNode(); i++) {

		XMLNode currNode = node.getChildNode(i);
		Message msg = Message("", PRIORITY_LOW, 0);

		std::vector<XMLUtils::Attribute> itemAttribs;
		XMLUtils::GetAllAttributesOfNode(currNode, itemAttribs);

		for (unsigned int j = 0; j < itemAttribs.size(); j++) {
			XMLUtils::Attribute& currAttrib = itemAttribs[j];
			String currAttribName = currAttrib.name;
			String currAttribVal = currAttrib.value;

			if (currAttribName == "text") {
				msg.m_message = currAttribVal;
			}
			else if (currAttribName == "turnNumber") {
				msg.m_turnNumber = XMLUtils::ParseIntFromXML(currAttribVal);
			}
		}

		s_loadedMessages->push_back(msg);
	}
}