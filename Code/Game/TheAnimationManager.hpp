#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Game/TileAnimation.hpp"

const int MAX_ANIMS_RUNNING = 100;

class TheAnimationManager;
extern TheAnimationManager* g_theAnimationManager;

class TheAnimationManager {
public:
	//STRUCTORS
	TheAnimationManager() {}

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	//INTERFACE
	void PlayAnimation(const IntVector2& position, const RGBA& color, char glyph);

private:
	TileAnimation m_anims[MAX_ANIMS_RUNNING];
};