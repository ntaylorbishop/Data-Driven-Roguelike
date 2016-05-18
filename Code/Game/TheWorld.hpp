#pragma once

#include "Engine/Renderer/Renderer/TheRenderer.hpp"
#include "Engine/General/Input/InputSystem.hpp"
#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Map.hpp"
#include "Game/Generator.hpp"
#include "Game/Player.hpp"
#include "Game/TileDefinition.hpp"

class PathFinder;
class TheWorld;
extern TheWorld* g_theWorld;

static std::vector<Vertex> s_renderVerts;
static std::vector<TexturedVertex> s_tRenderVerts;


class TheWorld {
public:
	//STRUCTORS
	TheWorld();
	~TheWorld();

	//INIT & CLEANUP
	void InitializeEntities();
	void InitializeEntitiesForLoadedGame();
	void FlushActiveEntities();
	void CleanUpDeadEntities();

	//UPDATE RENDER
	void Update(float deltaSeconds);

	void UpdateHoveringTileAndPathfindToIt();
	void UpdateTilePointers();

	void UpdateDeathAnim(float deltaSeconds);

	void RunNPCs();

	//RENDER
	void Render() const;

	void RenderItemsOnMap() const;
	void RenderEntities() const;
	void RenderTile(Tile* tile, const IntVector2& position) const;
	void RenderPathFind() const;

	//FOG OF WAR
	void UpdateFogOfWar();
	void DrawBressenhamLine(int octant, IntVector2 delta);
	IntVector2 SwitchOctant(int octant, IntVector2 p);

	//PATHFIND
	void DeleteCurrPathFinder();
	void CreateOrStepPathFind();


	//GETTERS SETTERS
	INLINE void SetMap(Map* map);
	uint GetEntityIDFromSavedID(uint savedID);

	void KillPlayer();

	void AddItemToEntityList(Item* item);

	//SAVE LOADING
	XMLNode SaveEntityData() const;

public:
	Map* m_activeMap;
	Tile* m_currHoveredTile;
	std::vector<Entity*> m_entitiesOnMap;
	Player* m_thePlayer;
	bool m_playerJustDied;
	bool m_exiting;
	float m_fadeAge;
	BitmapFont* m_squirrelFont;

private:
	//PRIVATE MEMBER FUNCTIONS
	void DebugRenderPathNode(const IntVector2& position, const RGBA& color) const;

	//BATCH DRAWING
	void RenderVerts() const;
	void AddAABB2Verts(const AABB2& quad, const RGBA& color) const;
	void AddAABB2Text(const AABB2& quad, const RGBA& color, char charToDraw) const;

private:
	float m_age;
	bool m_renderWholeMap;
	PathFinder* m_currPathFind;
	PathFinder* m_playerPathFind;


};

////////////////////////////////////////////////////
//------------------------------------------
inline void TheWorld::SetMap(Map* map) {
	m_activeMap = map;
}