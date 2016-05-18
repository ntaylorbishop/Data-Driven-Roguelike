#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Game/Agent.hpp"
#include "Game/Inventory.hpp"
#include "Game/Feature.hpp"

class TheInfoLog;
extern TheInfoLog* g_theInfoLog;

class TheInfoLog {
public:
	//STRUCTORS
	TheInfoLog();

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	void RenderHoverLog() const;

	//ANIM
	void PlaySaveAnimation(bool couldSave);
	void RenderSaveAnimation() const;
	void ClearAnimation();

private:
	void RenderNPCData(Agent* agent) const;
	void RenderItemData(Inventory* inv) const;
	void RenderItemData(Item* item) const;
	void RenderFeature(Feature* feature) const;

	//SAVE ANIM
	AABB2 m_healthBarQuad;
	float m_saveAge;
	RGBA m_saveColor;
	bool m_couldSave;
};