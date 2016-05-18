#pragma once

#include "Engine/Math/RangeInt.hpp"
#include "Game/Entity.hpp"

class Tile;
class Equipment;
class Inventory;
class Faction;
class Item;

//---------------------------------------------------------------------------------------------------------------------------
//AGENT CLASS
//---------------------------------------------------------------------------------------------------------------------------
class Agent : public Entity {
public:
	//STRUCTORS INIT
	Agent::Agent() : Entity(), m_isAlive(true) { }
	Agent::Agent(const IntVector2& position, char glyph, const RGBA& color, const RGBA& bgColor, const String& name, Map* const map)
		: Entity(position, glyph, color, bgColor, name, map)
		, m_moveSpeed(1.f)
		, m_faction(nullptr)
		, m_isAlive(true)
		, m_savedID(0)
	{ }

	void Initialize(int health, int maxHealth, RangeInt damage, float moveSpeed);

	//GETTERS SETTERS
	virtual bool IsReadyToUpdate() const = 0;

	void DecrementHealth(int amount);
	void IncrementHealth(int amount);
	int GetHealth() { return m_health; }
	void SetHealth(int amount) { m_health = amount; }
	int GetMaxHealth() { return m_maxHealth; }

	RangeInt GetDamage() { return m_damage; }
	void SetDamage(const RangeInt& dmg) { m_damage = dmg; }
	RangeInt GetPrevDamage() { return m_prevDamage; }
	void SetPrevDamage(const RangeInt& dmg) { m_prevDamage = dmg; }

	int GetArmor() const { return m_armor; }
	void SetArmor(int armor) { m_armor = armor; }

	bool IsAlive() { return m_isAlive; }
	float GetMoveSpeed() { return m_moveSpeed; }

	Faction* GetFaction() const { return m_faction; }
	void SetFactionID(Faction* factionID) { m_faction = factionID; }

	Equipment* GetEquipment() const { return m_equipment; }
	void SetEquipment(Equipment* equipment) { m_equipment = equipment; }
	Inventory* GetInventory() const { return m_inventory; }
	void SetInventory(Inventory* inv) { m_inventory = inv; }

	uint GetSavedID() const { return m_savedID; }
	void SetSavedID(uint savedID) { m_savedID = savedID; }

	bool IsAgent() const { return true; }

	virtual bool MoveToLocation(const IntVector2& loc);

	//LOGGING
	virtual void LogDeath() = 0;
	virtual void LogItemsPickedUp(Inventory* inv, bool didPickUp) = 0;

	virtual void LogItemPickup(Item* item) = 0;
	virtual void LogItemsPickup(Inventory* inv) = 0;
	virtual void LogItemCouldntPickup(Item* item) = 0;
	virtual void LogItemDrop(Item* item) = 0;
	virtual void LogItemUse(Item* item) = 0;
	virtual void LogEquipItem(Item* item) = 0;
	virtual void LogUnequipItem(Item* item, bool couldUnequip) = 0;

	//ITEM MANAGEMENT
	void PickUpItems(Tile* fromTile);

	void DropItem(Item* item);
	void UseItem(Item* item);

	void EquipItem(Item* item);
	void UnequipItem(Item* item);

protected:
	//INHERITED FROM ENTITY
	//IntVector2 m_position;
	//char m_glyph;
	//RGBA m_color;
	//RGBA m_backgroundColor;
	//String m_name;
	//uint m_id;
	//Map* const m_belongingMap;

	int m_health;
	int m_maxHealth;
	RangeInt m_damage;
	RangeInt m_prevDamage;
	bool m_isAlive;
	float m_moveSpeed;
	Faction* m_faction;
	int m_armor;

	Inventory* m_inventory;
	Equipment* m_equipment;

	uint m_savedID;
};