#include "Game/Equipment.hpp"
#include "Game/ItemFactory.hpp"
#include "Game/TheWorld.hpp"

void Equipment::Clone(Equipment* equipmentToClone) {
	if (equipmentToClone->GetWeaponInWeaponSlot()) {
		m_weaponSlot = new Item(equipmentToClone->GetWeaponInWeaponSlot());
	}
	if (equipmentToClone->GetArmorInArmorSlot()) {
		m_weaponSlot = new Item(equipmentToClone->GetArmorInArmorSlot());
	}
}

Item* Equipment::UnequipWeapon() {
	Item* itemPrevEquipped = m_weaponSlot;
	m_weaponSlot = nullptr;
	return itemPrevEquipped;
}
Item* Equipment::UnequipArmor() {
	Item* itemPrevEquipped = m_armorSlot;
	m_armorSlot = nullptr;
	return itemPrevEquipped;
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVE
//---------------------------------------------------------------------------------------------------------------------------
XMLNode Equipment::SaveToXML() const {
	XMLNode eqNode = eqNode.createXMLTopNode("Equipment");

	if (m_weaponSlot) {
		XMLNode wNode = wNode.createXMLTopNode("Weapon");
		wNode.addAttribute("name", m_weaponSlot->GetName().c_str());
		eqNode.addChild(wNode);
	}
	if (m_armorSlot) {
		XMLNode aNode = aNode.createXMLTopNode("Armor");
		aNode.addAttribute("name", m_armorSlot->GetName().c_str());
		eqNode.addChild(aNode);
	}

	return eqNode;
}

//---------------------------------------------------------------------------------------------------------------------------
//LOADING
//---------------------------------------------------------------------------------------------------------------------------
void Equipment::LoadFromXML(const XMLNode& node) {
	
	for (int i = 0; i < node.nChildNode(); i++) {
		XMLNode currNode = node.getChildNode(i);
		String nodeName = currNode.getAttribute(0).lpszName;
		String nodeVal = currNode.getAttribute(0).lpszValue;

		Item* nItem = ItemFactory::GetFactoryByName(nodeVal)->Create(g_theWorld->m_activeMap, IntVector2(-1, -1));

		if (nItem->GetType() == ITEM_WEAPON) {
			m_weaponSlot = nItem;
		}
		else if (nItem->GetType() == ITEM_ARMOR) {
			m_armorSlot = nItem;
		}
	}
}