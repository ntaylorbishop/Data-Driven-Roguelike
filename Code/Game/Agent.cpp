#include "Game/Agent.hpp"
#include "Game/Map.hpp"
#include "Game/Item.hpp"
#include "Game/Equipment.hpp"
#include "Game/Inventory.hpp"
#include "Game/Tile.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//INITALIZATION
//---------------------------------------------------------------------------------------------------------------------------
void Agent::Initialize(int health, int maxHealth, RangeInt damage, float moveSpeed) {
	m_health = health;
	m_maxHealth = maxHealth;
	m_damage = damage;
	m_moveSpeed = moveSpeed;
	m_isAlive = true;
	m_prevDamage = damage;
	m_armor = 0;

}

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
void Agent::DecrementHealth(int amount) {
	m_health -= amount;

	if (m_health <= 0) {
		m_health = 0;
		m_isAlive = false;
	}
}
void Agent::IncrementHealth(int amount) {
	m_health += amount;

	if (m_health > m_maxHealth) {
		m_health = m_maxHealth;
	}
}

VIRTUAL bool Agent::MoveToLocation(const IntVector2& loc) {
	Tile* tileToMoveTo = m_belongingMap->GetTileAtLocation(loc);

	
	if (tileToMoveTo && !tileToMoveTo->IsBlockingPathing() && !tileToMoveTo->GetOccupyingAgent()) {
		m_position = loc;
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//ITEM MANAGEMENT
//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void Agent::PickUpItems(Tile* fromTile) {

	Inventory* inv = fromTile->GetInventory();
	std::vector<Item*>* items = inv->GetItemsInBag();

	for (unsigned int i = 0; i < items->size(); i++) {
		Item* item = items->at(i);

		switch (item->GetType()) {
		case ITEM_WEAPON:
			if (m_equipment->GetWeaponInWeaponSlot()) {
				if (!m_inventory->IsInventoryFull()) {
					m_inventory->PutItemInBag(item);
					LogItemPickup(item);
				}
				else {
					LogItemCouldntPickup(item);
				}
			}
			else {
				m_equipment->EquipWeapon(item);
				LogEquipItem(item);
			}
			break;
		case ITEM_ARMOR:
			if (m_equipment->GetArmorInArmorSlot()) {
				if (!m_inventory->IsInventoryFull()) {
					m_inventory->PutItemInBag(item);
					LogItemPickup(item);
				}
				else {
					LogItemCouldntPickup(item);
				}
			}
			else {
				m_equipment->EquipArmor(item);
				LogEquipItem(item);
			}
			break;
		case ITEM_POTION:
			if (!m_inventory->IsInventoryFull()) {
				m_inventory->PutItemInBag(item);
				LogItemPickup(item);
			}
			else {
				LogItemCouldntPickup(item);
			}
			break;
		default:
			break;
		}
	}

	fromTile->ClearInventory();
}

VIRTUAL void Agent::DropItem(Item* item) {
	Tile* currTile = m_belongingMap->GetTileAtLocation(m_position);
	currTile->GetInventory()->PutItemInBag(item);

	LogItemDrop(item);
}
VIRTUAL void Agent::UseItem(Item* item) {
	switch (item->GetType()) {
	case ITEM_POTION:
		item->Use(this);
		m_inventory->RemoveItemFromBag(item);
		LogItemUse(item);
		break;
	default:
		break;
	}
}

VIRTUAL void Agent::EquipItem(Item* item) {

	switch (item->GetType()) {
	case ITEM_WEAPON:
		if (m_equipment->GetWeaponInWeaponSlot()) {
			m_inventory->RemoveItemFromBag(item); //Only remove if it was equipped from inventory
			m_inventory->PutItemInBag(m_equipment->GetWeaponInWeaponSlot());
			m_equipment->EquipWeapon(item);
			LogEquipItem(item);
		}
		else {
			m_inventory->RemoveItemFromBag(item); //Only remove if it was equipped from inventory
			m_equipment->EquipWeapon(item);
			LogEquipItem(item);
		}
		break;
	case ITEM_ARMOR:
		if (m_equipment->GetArmorInArmorSlot()) {
			m_inventory->RemoveItemFromBag(item); //Only remove if it was equipped from inventory
			m_inventory->PutItemInBag(m_equipment->GetArmorInArmorSlot());
			m_equipment->EquipArmor(item);
			LogEquipItem(item);
		}
		else {
			m_inventory->RemoveItemFromBag(item); //Only remove if it was equipped from inventory
			m_equipment->EquipArmor(item);
			LogEquipItem(item);
		}
		break;
	default:
		break;
	}
}
VIRTUAL void Agent::UnequipItem(Item* item) {

	switch (item->GetType()) {
	case ITEM_WEAPON:
		if (!m_inventory->IsInventoryFull()) {
			m_inventory->PutItemInBag(m_equipment->UnequipWeapon());
			LogUnequipItem(item, true);
		}
		else {
			LogUnequipItem(item, false);
		}
		m_equipment->RemoveWeapon();
		break;
	case ITEM_ARMOR:
		if (!m_inventory->IsInventoryFull()) {
			m_inventory->PutItemInBag(m_equipment->UnequipArmor());
			LogUnequipItem(item, true);
		}
		else {
			LogUnequipItem(item, false);
		}
		m_equipment->RemoveArmor();
		break;
	default:
		break;
	}
}