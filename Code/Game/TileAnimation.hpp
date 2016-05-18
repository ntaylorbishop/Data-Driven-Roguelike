#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/Math/IntVector2.hpp"

class TileAnimation {
public:
	//STRUCTORS
	TileAnimation() : m_position(IntVector2(-1, -1)), m_age(0.f), m_isAlive(false), m_glyph(' '), m_color(RGBA()) { }

	//PLAYING
	void PlayAnimation(const IntVector2& position, const RGBA& color, char glyph);

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	//GETTERS SETTERS
	bool IsAlive() const { return m_isAlive; }

private:
	IntVector2 m_position;
	float m_age;
	bool m_isAlive;
	RGBA m_color;
	char m_glyph;
};