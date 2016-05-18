#pragma once

#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"
#include "Game/Agent.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTS
//---------------------------------------------------------------------------------------------------------------------------
enum eItemType {
	ITEM_WEAPON = 0,
	ITEM_ARMOR,
	ITEM_POTION,
	ITEM_NUM_ITEM_TYPES
};

//---------------------------------------------------------------------------------------------------------------------------
//ITEM CLASS
//---------------------------------------------------------------------------------------------------------------------------
class Item : public Entity {
public:
	//STRUCTORS
	Item() { }
	Item(const XMLNode& itemNode);
	Item(Item* templateItem);
	
	void PopulateFromXMLNode(const XMLNode& itemNode);

	//UPDATE RENDER
	VIRTUAL void Update(float deltaSeconds);
	VIRTUAL void Render() const;
	VIRTUAL void Run();

	//GETTERS SETTERS
	VIRTUAL bool IsItem() const { return true; }
	eItemType GetType() const { return m_type; }
	RangeInt GetDamageBuff() const { return m_damageBuff; }
	int GetArmorBuff() const { return m_armorBuff; }
	int GetHealAmount() const { return m_healAmount; }

	String BonusToString() const;

	//USE
	void ApplyBuffs(Agent* agentToApplyTo);
	void RemoveBuffs(Agent* agentToApplyTo);
	void Use(Agent* agentToApplyTo);


	//SAVING LOADING
	VIRTUAL XMLNode SaveToXML() const;
	void LoadFromXML(const XMLNode& node);

	VIRTUAL void ResolveEntityPointers();

private:
	eItemType m_type;
	
	RangeInt m_damageBuff;
	int m_armorBuff;
	int m_healAmount;


	//INHERITED
	//IntVector2 m_position;
	//char m_glyph;
	//RGBA m_color;
	//RGBA m_backgroundColor;
	//String m_name;
	//uint m_id;
	//Map* m_belongingMap;
};