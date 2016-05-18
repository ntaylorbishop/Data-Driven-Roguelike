#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"

class Map;

class Entity {
public:
	//STRUCTORS
	Entity();
	Entity(const IntVector2& position, char glyph, const RGBA& color, const RGBA& bgColor, const String& name, Map* const map);

	virtual void Run();
	virtual void Update(float deltaSeconds) = 0;
	virtual void Render() const = 0;

	//GETTERS SETTERS
	IntVector2 GetPosition() const { return m_position; }
	void SetPosition(const IntVector2& position) { m_position = position; }

	char GetGlyph() const { return m_glyph; }
	void SetGlyph(char glyph) { m_glyph = glyph; }

	RGBA GetColor() const { return m_color; }
	void SetColor(const RGBA& col) { m_color = col; }

	RGBA GetBackgroundColor() const { return m_backgroundColor; }
	void SetBackgroundColor(const RGBA& bgColor) { m_backgroundColor = bgColor; }

	String GetName() const { return m_name; }
	void SetName(const String& name) { m_name = name; }

	uint GetEntityID() const { return m_id; }

	Map* GetBelongingMap() { return m_belongingMap; }
	void SetBelongingMap(Map* map) { m_belongingMap = map; }

	virtual bool IsPlayer() const { return false; }
	virtual bool IsAgent() const { return false; }
	virtual bool IsItem() const { return false; }

	//OPERATOR OVERLOADS
	bool operator==(const Entity& other);
	bool operator<(const Entity& other);

	virtual XMLNode SaveToXML() const = 0;
	virtual void ResolveEntityPointers() = 0;


protected:
	IntVector2 m_position;
	char m_glyph;
	RGBA m_color;
	RGBA m_backgroundColor;
	String m_name;
	uint m_id;
	Map* m_belongingMap;

	BitmapFont* m_squirrelFont;

//STATIC
public:
	static uint s_entityIDs;
};

//---------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////
//------------------------------------------
//INLINE
inline Entity::Entity()
	: m_position(IntVector2(-1, -1))
	, m_glyph(' ')
	, m_color(RGBA::WHITE)
	, m_backgroundColor(RGBA::WHITE)
	, m_name("")
	, m_id(s_entityIDs++)
	, m_belongingMap(nullptr)
	, m_squirrelFont(nullptr)
{
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFont");
}

inline Entity::Entity(const IntVector2& position, char glyph, const RGBA& color, const RGBA& bgColor, const String& name, Map* const map)
	: m_position(position)
	, m_glyph(glyph)
	, m_color(color)
	, m_backgroundColor(bgColor)
	, m_name(name)
	, m_id(s_entityIDs++)
	, m_belongingMap(map)
	, m_squirrelFont(nullptr)
{
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFont");
}

//OPERATOR OVERLOADS
inline bool Entity::operator==(const Entity& other) {
	return (m_id == other.GetEntityID()) ? (true) : (false);
}
inline bool Entity::operator<(const Entity& other) {
	return (m_id < other.GetEntityID());
}
