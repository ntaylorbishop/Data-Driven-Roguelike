#include "Game/TileAnimation.hpp"
#include "Game/TheWorld.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//PLAYING
//---------------------------------------------------------------------------------------------------------------------------
void TileAnimation::PlayAnimation(const IntVector2& position, const RGBA& color, char glyph) {
	m_position = position;
	m_color = RGBA(color.r, color.g, color.b, 1.f);
	m_position = position;
	m_age = 1.f;
	m_isAlive = true;
	m_glyph = glyph;
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TileAnimation::Update(float deltaSeconds) {
	m_age -= deltaSeconds;

	if (m_age <= 0.f) {
		m_isAlive = false;
	}
}
void TileAnimation::Render() const {
	RGBA colorToRender = RGBA(m_color.r, m_color.g, m_color.b, m_age);
	g_theWorld->m_activeMap->RenderGlyphAtLocation(m_glyph, m_position, colorToRender);
}