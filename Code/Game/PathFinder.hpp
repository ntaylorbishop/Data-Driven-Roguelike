#pragma once

#include <list>
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/MapProxy.hpp"


//---------------------------------------------------------------------------------------------------------------------------
//PATHNODE
//---------------------------------------------------------------------------------------------------------------------------
struct PathNode {
	float f;
	IntVector2 m_position;
	PathNode* m_parent;
	float gLocal;
	float gParent;
	float h;
};

//---------------------------------------------------------------------------------------------------------------------------
//PATHFINDER
//---------------------------------------------------------------------------------------------------------------------------
class PathFinder {
public:
	//STRUCTORS
	PathFinder();
	PathFinder(IntVector2 start, IntVector2 goal);
	~PathFinder();

	//PATHFIND
	std::vector<IntVector2> FindBestPathFromStartToGoal(IntVector2 start, IntVector2 goal, int movementProperties);
	bool RunOneStep(int movementProperties);

private:
	//PRIVATE UTILITY FUNCTIONS
	PathNode* CreateNode(IntVector2 position, PathNode* parent, float gLocal, float gParent, float h);
	void AddNodeToOpenList(PathNode* node);
	PathNode* FindLowestFNodeOnOpenListAndRemoveIt();
	void AddToClosedList(PathNode* node);
	void WalkClosedListAndConstructPath();
	bool IsPositionOnClosedList(IntVector2 pos);
	PathNode* FindOpenNodeWithPos(IntVector2 pos);
	void UpdateNodeValues(PathNode* nodeToUpdate, float gLocal, float gParent, PathNode* newParent);

	void CleanUpListsAndRestart();

public:
	bool m_isFinished;
	std::list<PathNode*> m_openList;
	std::list<PathNode*> m_closedList;

	IntVector2 m_start;
	IntVector2 m_goal;

	std::vector<IntVector2> m_currBestPath;
};