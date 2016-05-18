#pragma once

#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Game/State.hpp"

class MainMenuState : public State {
public:
	//STRUCTORS
	MainMenuState();

	//ENTER UPDATE RENDER EXIT
	void Enter();

	State* Update(float deltaSeconds);
	void UpdateMainMenu(float deltaSeconds);
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
};