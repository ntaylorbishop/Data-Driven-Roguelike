#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Game/State.hpp"
#include "Game/EnvironmentBlueprint.hpp"
#include "Game/TheWorld.hpp"
#include "Game/Generator.hpp"

class GeneratingState : public State {
public:
	//STRUCTORS
	GeneratingState(const String& environmentName, bool generateAllAtOnce);

	//ENTER UPDATE RENDER EXIT<
	void Enter();

	State* Update(float deltaSeconds);

	void UpdateGeneratingState(float deltaSeconds);
	void GenerateAllAtOnce();

	void Render() const;

	bool Exit();

private:
	State* SwitchStates();

private:
	//INHERITED:
	//String m_name;
	EnvironmentBlueprint* m_activeEnvironmentBlueprint;
	Generator* m_currGenerator;
	int m_currStepNumber;
	EnvironmentGeneratorData* m_currGenData;
	int m_currGenDataIdx;

	bool m_hasFinishedGenerating;
	bool m_generateAllAtOnce;
};