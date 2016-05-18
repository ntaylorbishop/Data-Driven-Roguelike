#include "Game/Faction.hpp"
#include "Game/NPC.hpp"

STATIC std::map<String, Faction*> Faction::s_globalFactions;
STATIC uint Faction::s_factionIDs = 0;

//---------------------------------------------------------------------------------------------------------------------------
//STATIC
//---------------------------------------------------------------------------------------------------------------------------
STATIC void Faction::LoadAllFactions() {

	String filePath = "Data/Factions/Global.Faction.xml";

	XMLNode root = XMLNode::parseFile(filePath.c_str());
	XMLNode factions = root.getChildNode(0);

	if (!factions.isEmpty()) {
		for (int fIdx = 0; fIdx < factions.nChildNode(); fIdx++) {
			XMLNode factionNode = factions.getChildNode(fIdx);

			std::vector<XMLUtils::Attribute> factionAttribs;
			XMLUtils::GetAllAttributesOfNode(factionNode, factionAttribs);

			String factionName = "";

			for (unsigned int attribIdx = 0; attribIdx < factionAttribs.size(); attribIdx++) {
				XMLUtils::Attribute& currAttrib = factionAttribs[attribIdx];
				StringUtils::ToLowerCase(currAttrib.name);

				if (currAttrib.name == "name") {
					StringUtils::ToLowerCase(currAttrib.value);
					factionName = currAttrib.value;
				}
			}

			ASSERT_OR_DIE(factionName != "", "ERROR PARSING XML: Faction must have a name.");

			s_globalFactions.insert(FactionMapPair(factionName, new Faction(factionNode)));
		}
	}
}

STATIC Faction* Faction::CreateFactionByName(const String& name) {
	String search = name;
	StringUtils::ToLowerCase(search);

	FactionMapIter factionIter = s_globalFactions.find(search);

	if (factionIter != s_globalFactions.end()) {
		Faction*& globalFaction = factionIter->second;

		return new Faction(search, globalFaction->GetFactionID(), globalFaction->GetFactionRelationships());
	}
	else {
		return nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Faction::Faction(const XMLNode& factionNode) 
	: m_factionID(s_factionIDs++)
	, m_name("NULL")
{
	PopulateFromXMLNode(factionNode);
}
Faction::Faction(const String& factionName, uint factionID, const std::map<String, eFactionRelationship>& factionRelationships) 
	: m_name(factionName)
	, m_factionID(factionID)
	, m_factionRelationships(factionRelationships)
{ }
Faction::Faction(Faction* faction) {
	m_factionID = faction->GetFactionID();
	m_name = faction->GetName();
	m_factionRelationships = faction->GetFactionRelationships();
	m_agentRelationships = faction->GetAgentRelationships();
}

void Faction::PopulateFromXMLNode(const XMLNode& factionNode) {

	std::vector<XMLUtils::Attribute> factionAttribs;
	XMLUtils::GetAllAttributesOfNode(factionNode, factionAttribs);

	ASSERT_OR_DIE(XMLUtils::DoesAttributeExist("name", factionAttribs), "ERROR: Factions must have a name.");

	m_name = factionAttribs[0].value;

	for (unsigned int i = 1; i < factionAttribs.size(); i++) {
		XMLUtils::Attribute& currAttrib = factionAttribs[i];

		StringUtils::ToLowerCase(currAttrib.name);

		eFactionRelationship currRelationship = NUM_FACTION_RELATIONSHIPS;

		if (currAttrib.name == "loves") {
			currRelationship = FACTION_RELATIONSHIP_LOVES;
		}
		else if (currAttrib.name == "likes") {
			currRelationship = FACTION_RELATIONSHIP_LIKES;
		}
		else if (currAttrib.name == "neutral") {
			currRelationship = FACTION_RELATIONSHIP_NUETRAL;
		}
		else if (currAttrib.name == "dislikes") {
			currRelationship = FACTION_RELATIONSHIP_DISLIKES;
		}
		else if (currAttrib.name == "hates") {
			currRelationship = FACTION_RELATIONSHIP_HATES;
		}

		if (currRelationship != NUM_FACTION_RELATIONSHIPS) {
			std::vector<String> factionsToChange;
			StringUtils::TokenizeBuffer(currAttrib.value, ',', factionsToChange);

			for (unsigned int j = 0; j < factionsToChange.size(); j++) {
				String& currFactionToChange = factionsToChange[j];

				StringUtils::ToLowerCase(currFactionToChange);
				AddFactionRelationships(currFactionToChange, currRelationship);
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//RELATIONSHIPS
//---------------------------------------------------------------------------------------------------------------------------
eFactionRelationship Faction::GetFactionRelationship(const String& factionName) const {
	FactionRelationshipConstIter relIter = m_factionRelationships.find(factionName);

	return (relIter != m_factionRelationships.end()) ? (relIter->second) : (FACTION_RELATIONSHIP_NUETRAL);
}
void Faction::OverrideFactionRelationship(const String& factionName, eFactionRelationship newRelationship) {
	FactionRelationshipIter relIter = m_factionRelationships.find(factionName);

	if (relIter != m_factionRelationships.end()) {
		relIter->second = newRelationship;
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void Faction::ChangeRelationshipWithAgent(const Agent* agent, eRelationshipChange change) {
	uint agentID = agent->GetEntityID();

	AgentRelationshipIter relIter = m_agentRelationships.find(agentID);


	if (relIter != m_agentRelationships.end()) {
		eFactionRelationship currRelationship = relIter->second;

		if (change == FACTION_RELATIONSHIP_POSITIVE) {
			switch (currRelationship) {
			case FACTION_RELATIONSHIP_LIKES:
				relIter->second = FACTION_RELATIONSHIP_LOVES;
				break;
			case FACTION_RELATIONSHIP_NUETRAL:
				relIter->second = FACTION_RELATIONSHIP_LIKES;
				break;
			case FACTION_RELATIONSHIP_DISLIKES:
				relIter->second = FACTION_RELATIONSHIP_NUETRAL;
				break;
			case FACTION_RELATIONSHIP_HATES:
				relIter->second = FACTION_RELATIONSHIP_DISLIKES;
				break;
			}
		}
		else if (change == FACTION_RELATIONSHIP_NEGATIVE) {
			switch (currRelationship) {
			case FACTION_RELATIONSHIP_LOVES:
				relIter->second = FACTION_RELATIONSHIP_LIKES;
				break;
			case FACTION_RELATIONSHIP_LIKES:
				relIter->second = FACTION_RELATIONSHIP_NUETRAL;
				break;
			case FACTION_RELATIONSHIP_NUETRAL:
				relIter->second = FACTION_RELATIONSHIP_DISLIKES;
				break;
			case FACTION_RELATIONSHIP_DISLIKES:
				relIter->second = FACTION_RELATIONSHIP_HATES;
				break;
			}
		}
	}
	else {
		if (change == FACTION_RELATIONSHIP_POSITIVE) {
			m_agentRelationships.insert(AgentRelationshipPair(agentID, FACTION_RELATIONSHIP_LIKES));
		}
		else if (change == FACTION_RELATIONSHIP_NEGATIVE) {
			m_agentRelationships.insert(AgentRelationshipPair(agentID, FACTION_RELATIONSHIP_DISLIKES));
		}
	}
}
eFactionRelationship Faction::GetAgentRelationship(Agent* agent) {
	uint agentID = agent->GetEntityID();

	AgentRelationshipIter relIter = m_agentRelationships.find(agentID);

	if (relIter != m_agentRelationships.end()) {
		return relIter->second;
	}
	else {
		return GetFactionRelationship(agent->GetFaction()->GetName());
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
void Faction::AddFactionRelationships(const String& tokens, eFactionRelationship relationship) {
	std::vector<String> factionNames;
	StringUtils::TokenizeBuffer(tokens, ',', factionNames);

	for (unsigned int j = 0; j < factionNames.size(); j++) {
		String& factionName = factionNames[j];

		StringUtils::ToLowerCase(factionName);
		m_factionRelationships.insert(FactionRelationshipPair(factionName, relationship));
	}
}