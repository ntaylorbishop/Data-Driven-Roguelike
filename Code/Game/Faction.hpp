#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/Agent.hpp"

class Faction;

//---------------------------------------------------------------------------------------------------------------------------
//ENUMS
//---------------------------------------------------------------------------------------------------------------------------
enum eFactionRelationship {
	FACTION_RELATIONSHIP_LOVES = 0,
	FACTION_RELATIONSHIP_LIKES,
	FACTION_RELATIONSHIP_NUETRAL,
	FACTION_RELATIONSHIP_DISLIKES,
	FACTION_RELATIONSHIP_HATES,
	NUM_FACTION_RELATIONSHIPS
};

enum eRelationshipChange {
	FACTION_RELATIONSHIP_POSITIVE = 0,
	FACTION_RELATIONSHIP_NEGATIVE,
};

//---------------------------------------------------------------------------------------------------------------------------
//CONSTANTS
//---------------------------------------------------------------------------------------------------------------------------
const int RELATIONSHIP_HATES = -10;
const int RELATIONSHIP_DISLIKES = -5;
const int RELATIONSHIP_NEUTRAL = 0;
const int RELATIONSHIP_LIKES = 5;
const int RELATIONSHIP_LOVES = 10;


//---------------------------------------------------------------------------------------------------------------------------
//TYPEDEFS
//---------------------------------------------------------------------------------------------------------------------------
typedef std::pair<String, eFactionRelationship> FactionRelationshipPair;
typedef std::map<String, eFactionRelationship>::const_iterator FactionRelationshipConstIter;
typedef std::map<String, eFactionRelationship>::iterator FactionRelationshipIter;

typedef std::pair<uint, eFactionRelationship> AgentRelationshipPair;
typedef std::map<uint, eFactionRelationship>::const_iterator AgentRelationshipConstIter;
typedef std::map<uint, eFactionRelationship>::iterator AgentRelationshipIter;

typedef std::map<String, Faction*>::iterator FactionMapIter;
typedef std::pair<String, Faction*> FactionMapPair;

//---------------------------------------------------------------------------------------------------------------------------
//FACTION CLASS
//---------------------------------------------------------------------------------------------------------------------------
class Faction {
//MEMBER
public:
	//STRUCTORS
	Faction(const XMLNode& factionNode);
	Faction(const String& factionName, uint factionID, const std::map<String, eFactionRelationship>& factionRelationships);
	Faction(Faction* faction);

	void PopulateFromXMLNode(const XMLNode& factionNode);

	//RELATIONSHIPS
	eFactionRelationship GetFactionRelationship(const String& factionName) const;
	void OverrideFactionRelationship(const String& factionName, eFactionRelationship newRelationship);

	void ChangeRelationshipWithAgent(const Agent* agent, eRelationshipChange change);
	eFactionRelationship GetAgentRelationship(Agent* agent);

	//GETTERS SETTERS
	String GetName() const { return m_name; }
	uint GetFactionID() const { return m_factionID; }
	std::map<String, eFactionRelationship> GetFactionRelationships() const { return m_factionRelationships; }
	std::map<uint, eFactionRelationship> GetAgentRelationships() const { return m_agentRelationships; }

private:
	//PRIVATE MEMBER FUNCTIONS
	void AddFactionRelationships(const String& tokens, eFactionRelationship relationship);

	//MEMBER DATA
	std::map<String, eFactionRelationship> m_factionRelationships;
	std::map<uint, eFactionRelationship> m_agentRelationships;
	String m_name;
	uint m_factionID;

//STATIC
public:
	static void LoadAllFactions();
	static Faction* CreateFactionByName(const String& name);

private:
	static std::map<String, Faction*> Faction::s_globalFactions;
	static uint s_factionIDs;
};