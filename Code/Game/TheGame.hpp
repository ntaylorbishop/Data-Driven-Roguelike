#pragma once

#include "Engine/General/Core/GameEngine.hpp"
#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Game/StateMachine.hpp"
#include "Game/EventSystem.hpp"
#include "Game/TheWorld.hpp"
#include "Game/MenuStateMachine.hpp"
#include "Game/EnvironmentBlueprint.hpp"
#include "Game/NPCFactory.hpp"
#include "Game/TheMessageLog.hpp"
#include "Game/Faction.hpp"
#include "Game/ItemFactory.hpp"
#include "Game/FeatureFactory.hpp"
#include "Game/TileDefinition.hpp"
#include "Game/TheSaveLoadManager.hpp"

class TheGame;
extern TheGame* g_theGame;

class TheGame {
public:
	//STRUCTORS INITIALIZATION
	TheGame();
	~TheGame();

	void InitializeData();
	void RegisterCommands();

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	void ExitGame();

	uint m_turnCountPlayerDied;
	uint m_numMonstersKilled;

private:
	MenuStateMachine m_menuStateMachine;
};

//CONSOLE COMMANDS
void AddMessageToMessageLog(Command& args);