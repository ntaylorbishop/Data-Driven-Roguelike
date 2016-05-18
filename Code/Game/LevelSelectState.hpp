#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Engine/General/UI/UIButton.hpp"
#include "Game/State.hpp"
#include "Game/EnvironmentBlueprint.hpp"
#include "Game/TheSaveLoadManager.hpp"

class LevelSelectState : public State {
public:
	//STRUCTORS
	LevelSelectState();

	//ENTER UPDATE RENDER EXIT
	void Enter();

	State* Update(float deltaSeconds);
	void UpdateLevelSelect(float deltaSeconds);
	
	void Render() const;

	bool Exit();

private:
	State* SwitchStates();

private:
	//INHERITED:
	//String m_name;
	
	Texture* m_screenImage;
	float m_age;
	BitmapFont* m_squirrelFont;

	bool m_shouldGenerateAllAtOnce;
	float m_tabSwitchAge;

	std::vector<String> m_environments;

	//MOUSE INPUT
	std::vector<UIButton> m_rects;
};