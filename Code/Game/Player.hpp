#pragma once

#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Engine/Renderer/Fonts/Font.hpp"
#include "Engine/General/Input/InputSystem.hpp"
#include "Engine/Math/RangeInt.hpp"
#include "Game/Agent.hpp"

class PathFinder;

//---------------------------------------------------------------------------------------------------------------------------
//CONSTANTS
//---------------------------------------------------------------------------------------------------------------------------
const int PLAYER_HEALTH = 10;
const int PLAYER_ATTACK = 5;

class Player : public Agent {
public:
	//STRUCTORS INIT
	Player(char glyph, const RGBA& color, const RGBA& bgColor, Map* map);
	Player(const XMLNode& playerDataNode);

	static void InitializePlayerData();

	bool IsReadyToUpdate() const { return m_isReadyToUpdate; }
	bool CanSeeEntity(Entity* entity);
	
	//UPDATE RENDER
	void Run();
	void Update(float deltaSeconds);
	void Render() const;

	//INPUT
	void HandleInput();
	void HandleFeatureToggle();

	bool IsPlayer() const { return true; }

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

	//SAVE LOADING
	VIRTUAL XMLNode SaveToXML() const;
	void LoadFromXML(const XMLNode& playerDataNode);

	VIRTUAL void ResolveEntityPointers() { }

public:
	PathFinder* m_playerPathFind;
	std::vector<Entity*> m_visibleEntities;

private:
	BitmapFont* m_squirrelFont;
	bool m_isReadyToUpdate;
	Tile* m_tileToMoveTo;
	IntVector2 m_nextLoc;

	////////////////////////////////////////////////////
	//------------------------------------------
	//INHERITED FROM ENTITY
	//IntVector2 m_position;
	//char m_glyph;
	//RGBA m_color;
	//RGBA m_backgroundColor;
	//String m_name;
	//uint m_id;
	//Map* const m_belongingMap;

	////////////////////////////////////////////////////
	//------------------------------------------
	//INHERITED FROM AGENT
	//int m_health;
	//int m_maxHealth;
	//RangeInt m_damage;
	//bool m_isAlive;
	//Faction* m_faction;

	static int s_playerMaxHealth;
	static RangeInt s_playerDamage;
};