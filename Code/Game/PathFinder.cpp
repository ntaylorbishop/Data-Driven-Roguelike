#include "Game/PathFinder.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
PathFinder::PathFinder() 
	: m_start()
	, m_goal()
	, m_isFinished(false)
{ }
PathFinder::PathFinder(IntVector2 start, IntVector2 goal) 
	: m_start(start)
	, m_goal(goal)
	, m_isFinished(false)
{ 
	PathNode* startNode = CreateNode(start, nullptr, 0.f, 0.f, 0.f);
	startNode->h = MapProxy::ComputeHueristic(startNode->m_position, m_goal);
	startNode->f = startNode->h;
	AddNodeToOpenList(startNode);
}
PathFinder::~PathFinder() {
	CleanUpListsAndRestart();
}

//---------------------------------------------------------------------------------------------------------------------------
//PATHFIND
//---------------------------------------------------------------------------------------------------------------------------
std::vector<IntVector2> PathFinder::FindBestPathFromStartToGoal(IntVector2 start, IntVector2 goal, int movementProperties) {

	if (start == goal)
		return std::vector<IntVector2>();

	m_start = start;
	m_goal = goal;

	CleanUpListsAndRestart();

	PathNode* startNode = CreateNode(start, nullptr, 0.f, 0.f, 0.f);
	startNode->h = MapProxy::ComputeHueristic(startNode->m_position, m_goal);
	startNode->f = startNode->h;
	AddNodeToOpenList(startNode);

	while (!m_isFinished) {
		if (!RunOneStep(movementProperties)) {
			return std::vector<IntVector2>();
		}
	}

	return m_currBestPath;
}
//---------------------------------------------------------------------------------------------------------------------------
bool PathFinder::RunOneStep(int movementProperties) {
	if(m_openList.empty())
		return false;

	PathNode* activeNode = FindLowestFNodeOnOpenListAndRemoveIt();
	AddToClosedList(activeNode);

	if (activeNode->m_position == m_goal) {
		WalkClosedListAndConstructPath();
		m_isFinished = true;
		return true;
	}

	std::vector<IntVector2> validPositions = MapProxy::FindValidAdjacentPositions(activeNode->m_position, movementProperties);

	for (IntVector2& position : validPositions) {
		if (IsPositionOnClosedList(position))
			continue;
		float gLocal = MapProxy::ComputeLogalG(activeNode->m_position, position, movementProperties);
		float gParent = activeNode->gLocal + activeNode->gParent;
		float h = MapProxy::ComputeHueristic(position, m_goal);

		PathNode* existingNode = FindOpenNodeWithPos(position);
		if (existingNode) {
			if ((gLocal + gParent) < (existingNode->gLocal + existingNode->gParent)) {
				UpdateNodeValues(existingNode, gLocal, gParent, activeNode);
			}
				continue;
		}

		PathNode* newNode = CreateNode(position, activeNode, gLocal, gParent, h);
		AddNodeToOpenList(newNode);
	}

	return false;
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE UTILITY FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
PathNode* PathFinder::CreateNode(IntVector2 position, PathNode* parent, float gLocal, float gParent, float h) {
	PathNode* node = new PathNode();
	node->m_position = position;
	node->m_parent = parent;
	node->gLocal = gLocal;

	if (!parent) {
		node->gParent = gParent;
	}
	else {
		node->gParent = parent->gParent + parent->gLocal;
	}

	node->gParent = gParent;
	node->h = h;
	node->f = gLocal + gParent + h;
	return node;
}
void PathFinder::AddNodeToOpenList(PathNode* node) {
	m_openList.push_back(node);
}
PathNode* PathFinder::FindLowestFNodeOnOpenListAndRemoveIt() {
	ASSERT_OR_DIE(!m_openList.empty(), "ERROR: OPEN LIST IS EMPTY");
	PathNode* currNode = m_openList.front();

	for (std::list<PathNode*>::iterator it = m_openList.begin(); it != m_openList.end(); ++it) {
		if (it == m_openList.begin()) {
			continue;
		}

		PathNode* node = *it;
		if (node->f < currNode->f) {
			currNode = node;
		}
	}

	m_openList.remove(currNode);
	return currNode;
}
void PathFinder::AddToClosedList(PathNode* node) {
	m_closedList.push_back(node);
}
void PathFinder::WalkClosedListAndConstructPath() {
	ASSERT_OR_DIE(!m_closedList.empty(), "ERROR: CAN'T CONSTRUCT PATH BECAUSE CLOSED LIST IS EMPTY");

	PathNode* currNode = m_closedList.back();

	while (nullptr != currNode) {
		m_currBestPath.push_back(currNode->m_position);
		currNode = currNode->m_parent;
	}
}
bool PathFinder::IsPositionOnClosedList(IntVector2 pos) {
	
	bool found = false;

	for (std::list<PathNode*>::iterator it = m_closedList.begin(); it != m_closedList.end(); ++it) {
		PathNode* currNode = *it;

		if (currNode->m_position == pos) {
			found = true;
		}
	}

	return found;
}
PathNode* PathFinder::FindOpenNodeWithPos(IntVector2 pos) {
	PathNode* foundNode = nullptr;

	for (std::list<PathNode*>::iterator it = m_openList.begin(); it != m_openList.end(); ++it) {
		PathNode* currNode = *it;

		if (currNode->m_position == pos) {
			foundNode = currNode;
		}
	}

	return foundNode;
}
void PathFinder::UpdateNodeValues(PathNode* nodeToUpdate, float gLocal, float gParent, PathNode* newParent) {
	nodeToUpdate->gLocal = gLocal;
	nodeToUpdate->gParent = gParent;
	nodeToUpdate->m_parent = newParent;
	nodeToUpdate->f = gLocal + gParent + nodeToUpdate->h;
}

void PathFinder::CleanUpListsAndRestart() {
	m_currBestPath.clear();
	m_isFinished = false;

	for (std::list<PathNode*>::iterator it = m_openList.begin(); it != m_openList.end(); ++it) {
		delete *it;
	}
	for (std::list<PathNode*>::iterator it = m_closedList.begin(); it != m_closedList.end(); ++it) {
		delete *it;
	}

	m_openList.clear();
	m_closedList.clear();
}