#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Game/Item.hpp"

class Inventory {
public:
	//STRUCTORS / CLONING
	Inventory() : m_belongsToEntity(false) { }
	Inventory(bool belongsToEntity) : m_belongsToEntity(belongsToEntity) { }

	void Clone(Inventory* inv);

	//SETTING
	void PutItemInBag(Item* item) { m_itemsContained.push_back(item); }
	void PutItemsInBag(const std::vector<Item*>& items) { m_itemsContained.insert(m_itemsContained.end(), items.begin(), items.end()); }
	void SetItemsInBag(const std::vector<Item*>& items) { m_itemsContained = items; }
	void RemoveItemFromBag(Item* item);

	//GETTING
	std::vector<Item*>* GetItemsInBag() { return &m_itemsContained; }
	bool IsInventoryFull() const { return (m_itemsContained.size() >= MAX_NUM_ITEMS_INVENTORY); }
	bool IsInventoryEmpty() const { return m_itemsContained.empty(); }
	Item* GetItemInSlot(int idx) const;

	//RENDER
	void Render(Map* map, const IntVector2& loc) const;

	//SAVE LOADING
	void LoadFromXML(const XMLNode& node);
	XMLNode SaveToXML(const IntVector2& loc) const;

private:
	//LOADING
	void ParseItemFromXML(const XMLNode& node);


	std::vector<Item*> m_itemsContained;

	bool m_belongsToEntity;
};