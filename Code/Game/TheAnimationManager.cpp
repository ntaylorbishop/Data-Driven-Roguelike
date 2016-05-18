#include "Game/TheAnimationManager.hpp"

TheAnimationManager* g_theAnimationManager = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheAnimationManager::Update(float deltaSeconds) {
	for (unsigned int i = 0; i < MAX_ANIMS_RUNNING; i++) {
		if (m_anims[i].IsAlive()) {
			m_anims[i].Update(deltaSeconds);
		}
	}
}
void TheAnimationManager::Render() const {
	for (unsigned int i = 0; i < MAX_ANIMS_RUNNING; i++) {
		if (m_anims[i].IsAlive()) {
			m_anims[i].Render();
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//INTERFACE
//---------------------------------------------------------------------------------------------------------------------------
void TheAnimationManager::PlayAnimation(const IntVector2& position, const RGBA& color, char glyph) {
	for (unsigned int i = 0; i < MAX_ANIMS_RUNNING; i++) {
		if (!m_anims[i].IsAlive()) {
			m_anims[i].PlayAnimation(position, color, glyph);
			break;
		}
	}
}