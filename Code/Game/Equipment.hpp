#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Game/Item.hpp"

class Equipment {
public:
	Equipment() : m_weaponSlot(nullptr), m_armorSlot(nullptr) { }

	void Clone(Equipment* equipmentToClone);

	Item* GetWeaponInWeaponSlot() const { return m_weaponSlot; }
	Item* GetArmorInArmorSlot() const { return m_armorSlot; }

	void EquipWeapon(Item* wep) { m_weaponSlot = wep; }
	void EquipArmor(Item* armor) { m_armorSlot = armor; }

	Item* UnequipWeapon();
	Item* UnequipArmor();

	void RemoveWeapon() { m_weaponSlot = nullptr; }
	void RemoveArmor() { m_armorSlot = nullptr; }

	//SAVE LOADING
	XMLNode SaveToXML() const;
	void LoadFromXML(const XMLNode& node);

private:
	Item* m_weaponSlot;
	Item* m_armorSlot;
};