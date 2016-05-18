#pragma once

#include <set>

#include "ThirdParty/Parsers/xmlParser.h"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/General/Utils/GeneralUtils/BitUtils.hpp"
#include "Engine/General/Utils/StringUtils.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/Agent.hpp"
#include "Game/Behavior.hpp"
#include "Game/Faction.hpp"
#include "Game/TheCombatSystem.hpp"
#include "Game/Item.hpp"

class PathFinder;

//---------------------------------------------------------------------------------------------------------------------------
//ENUMS
//---------------------------------------------------------------------------------------------------------------------------
enum eMovementProperties {
	BLOCKED_BY_LAVA = 1,
	SLOWED_BY_LAVA = 2,
	BLOCKED_BY_WATER = 4,
	SLOWED_BY_WATER = 8,
	BLOCKED_BY_WALLS = 16,
	BLOCKED_BY_AGENTS = 32,

	IS_LAVA = 1,
	IS_WATER = 4,
	IS_WALL = 16,
	CONTAINS_AGENT = 32
};

static const int PLAYER_MOVEMENT_PROPERTIES = BLOCKED_BY_WALLS | BLOCKED_BY_AGENTS | BLOCKED_BY_LAVA;

//---------------------------------------------------------------------------------------------------------------------------
//NPC CLASS
//---------------------------------------------------------------------------------------------------------------------------
class NPC : public Agent {
public:
	//STRUCTORS
	NPC() { }
	NPC(const XMLNode& node);
	NPC(NPC* templateNPC);

	bool IsReadyToUpdate() const { return true; }

	//UPDATE RENDER
	void Run();
	void Update(float deltaSeconds);
	void Render() const;

	//LOS & PATHFIND
	void GetEntitiesInLineOfSight();
	void DrawBressenhamLine(int octant, IntVector2 delta);
	IntVector2 SwitchOctant(int octant, IntVector2 p);

	//GETTERS SETTERS
	int GetMaxHealth() const { return m_maxHealth; }
	void SetMaxHealth(int maxHealth) { m_maxHealth = maxHealth; }
	int GetHealth() const { return m_health; }
	void SetHealth(int health) { m_health = health; }

	void GetAdjacentAgentsWithinManhattanDistance(int dist, std::vector<Agent*>& outVec) const;
	void GetAdjacentItemsWithinManhattanDistance(int dist, std::vector<IntVector2>& outVec) const;

	int GetMovementProperties() const { return m_movementProperties; }

	VIRTUAL bool MoveToLocation(const IntVector2& loc);
	
	//COMBAT
	void NotifyAttacked(const AttackData& data);

	//LOGGING
	VIRTUAL void LogDeath();
	VIRTUAL void LogItemsPickedUp(Inventory* inv, bool didPickUp);

	VIRTUAL void LogItemPickup(Item* item);
	VIRTUAL void LogItemsPickup(Inventory* inv);
	VIRTUAL void LogItemCouldntPickup(Item* item);
	VIRTUAL void LogItemDrop(Item* item);
	VIRTUAL void LogItemUse(Item* item);
	VIRTUAL void LogEquipItem(Item* item);
	VIRTUAL void LogUnequipItem(Item* item, bool couldUnequip);


	//SAVING LOADING
	VIRTUAL XMLNode SaveToXML() const;
	void LoadFromXML(const XMLNode& node);

	VIRTUAL void ResolveEntityPointers();





	std::set<Entity*> m_entitiesInView;
	std::set<IntVector2> m_itemsInView;
	Agent* m_currTargetedEnemy;
	Agent* m_currTargetedAlly;
	Item* m_currTargetedItem;
	PathFinder* m_currPathFind;

private:
	//FOR LOADING
	void ParseLoadedAgentRelationships(const XMLNode& node);

	//PRIVATE MEMBER FUNCTIONS
	void PopulateFromXMLNode(const XMLNode& node);
	void ParseFactionOverride(const XMLNode& factionOverrideNode);
	void ParseBehaviors(const XMLNode& behaviorsNode);
	void ParseInventory(const XMLNode& invNode);
	void ParseEquipment(const XMLNode& equipmentNode);
	void ParseMovementProperties(const XMLNode& movementProperties);

	//PRIVATE DATA MEMBERS
	std::vector<Behavior*> m_behaviors;
	std::map<uint, eFactionRelationship> m_agentRelationships;
	std::vector<std::pair<uint, eFactionRelationship>> m_loadedAgentRelationships;
	int m_movementProperties;


	//INHRETID FROM AGENT
	//int m_health;
	//int m_maxHealth;
	//RangeInt m_damage;
	//bool m_isAlive;
	//float m_moveSpeed;

	//INHERITED FROM ENTITY
	//IntVector2 m_position;
	//char m_glyph;
	//RGBA m_color;
	//RGBA m_backgroundColor;
	//String m_name;
	//uint m_id;
	//Map* m_belongingMap;
	//Faction* m_faction;
};