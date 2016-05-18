#include "Game/Inventory.hpp"
#include "Game/Map.hpp"
#include "Game/TheWorld.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS / CLONING
//---------------------------------------------------------------------------------------------------------------------------
void Inventory::Clone(Inventory* inv) {
	
	std::vector<Item*>* itemsToClone = inv->GetItemsInBag();

	for (unsigned int i = 0; i < itemsToClone->size(); i++) {
		Item* itemToAdd = new Item(itemsToClone->at(i));
		m_itemsContained.push_back(itemToAdd);
		g_theWorld->AddItemToEntityList(itemToAdd);
	}

	m_belongsToEntity = true;
}

//---------------------------------------------------------------------------------------------------------------------------
//RENDER
//---------------------------------------------------------------------------------------------------------------------------
void Inventory::Render(Map* map, const IntVector2& loc) const {
	if (m_itemsContained.size() == 1) {
		Item* itemToRender = m_itemsContained[0];

		map->RenderGlyphAtLocation(itemToRender->GetGlyph(), loc, RGBA::WHITE);
	}
	else if (m_itemsContained.size() > 1) {
		map->RenderGlyphAtLocation('*', loc, RGBA::WHITE);
	}
}

void Inventory::RemoveItemFromBag(Item* item) {
	for (unsigned int i = 0; i < m_itemsContained.size(); i++) {
		if (m_itemsContained[i] == item) {
			m_itemsContained.erase(m_itemsContained.begin() + i);
			return;
		}
	}

	DebuggerPrintf("Could not find item to remove from bag.");
}

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
Item* Inventory::GetItemInSlot(int idx) const {
	if (idx >= (int)m_itemsContained.size()) {
		return nullptr;
	}
	else {
		return m_itemsContained[idx];
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVE
//---------------------------------------------------------------------------------------------------------------------------
XMLNode Inventory::SaveToXML(const IntVector2& loc) const {
	XMLNode invNode = invNode.createXMLTopNode("Inventory");

	invNode.addAttribute("position", loc.ToString().c_str());

	for (unsigned int i = 0; i < m_itemsContained.size(); i++) {
		XMLNode iNode = iNode.createXMLTopNode("Item");
		iNode.addAttribute("name", m_itemsContained[i]->GetName().c_str());
		invNode.addChild(iNode);
	}

	return invNode;
}

//---------------------------------------------------------------------------------------------------------------------------
//LOADING
//---------------------------------------------------------------------------------------------------------------------------
void Inventory::LoadFromXML(const XMLNode& node) {

	XMLAttribute posAttrib = node.getAttribute(0);
	String posAttribVal = posAttrib.lpszValue;

	IntVector2 pos = XMLUtils::ParseIntVector2FromXML(posAttribVal);

	for (int i = 0; i < node.nChildNode(); i++) {
		XMLNode currNode = node.getChildNode(i);
		ParseItemFromXML(currNode);
	}

	if (pos != IntVector2(-1, -1)) {
		Tile* tileToPlaceOn = g_theWorld->m_activeMap->GetTileAtLocation(pos);
		tileToPlaceOn->SetInventory(this);
	}
}

void Inventory::ParseItemFromXML(const XMLNode& node) {

	XMLAttribute posAttrib = node.getAttribute(0);
	String posAttribVal = posAttrib.lpszValue;

	Item* itemToAdd = ItemFactory::GetFactoryByName(posAttribVal)->Create(g_theWorld->m_activeMap, IntVector2(-1, -1));
	m_itemsContained.push_back(itemToAdd);
	g_theWorld->m_entitiesOnMap.push_back(itemToAdd);

}