#include "Game/TheGame.hpp"

TheGame* g_theGame = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS INITIALIZATION
//---------------------------------------------------------------------------------------------------------------------------
TheGame::TheGame() 
	: m_numMonstersKilled(0)
	, m_turnCountPlayerDied(0)
{
	g_theWorld = new TheWorld();
	m_menuStateMachine = MenuStateMachine();
	g_theSaveLoadManager = new TheSaveLoadManager();

	TileDefinition::InitializeTileDefinitions();
	InitializeData();
	RegisterCommands();
}
TheGame::~TheGame() {
	delete g_theSaveLoadManager;
	g_theSaveLoadManager = nullptr;
	delete g_theWorld;
	g_theWorld = nullptr;
}

void TheGame::InitializeData() {
	EnvironmentBlueprint::LoadEnvironmentBlueprints();
	Faction::LoadAllFactions();
	ItemFactory::LoadAllItems();
	NPCFactory::LoadAllNPCs();
	FeatureFactory::LoadAllFeatures();
	Player::InitializePlayerData();
}
void TheGame::RegisterCommands() {
	g_theApparatusEngine->BindKeyToConsole(VK_F1);

	g_theApparatusEngine->RegisterCommandToConsole("logmessage", AddMessageToMessageLog);
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheGame::Update(float deltaSeconds) {
	m_menuStateMachine.Update(deltaSeconds);
}
void TheGame::Render() const {
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(1600.f, 900.f));

	m_menuStateMachine.Render();
}

////////////////////////////////////////////////////
//------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//CONSOLE COMMANDS
//---------------------------------------------------------------------------------------------------------------------------
void AddMessageToMessageLog(Command& args) {
	String strToAdd = " ";

	String currStr;
	while (args.GetNextString(currStr)) {
		strToAdd += currStr + " ";
	}

	g_theMessageLog->LogMessage(strToAdd, PRIORITY_HIGH);
	g_theApparatusEngine->PrintOutputToConsole("Added message to message log.", CONSOLE_VERIFY);
}