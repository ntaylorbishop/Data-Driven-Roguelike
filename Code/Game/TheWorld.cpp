#include "Game/TheWorld.hpp"
#include "Game/PathFinder.hpp"
#include "Game/TheGame.hpp"
#include "Game/NPC.hpp"

TheWorld* g_theWorld = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
TheWorld::TheWorld() 
	: m_activeMap(nullptr)
	, m_thePlayer(nullptr)
	, m_renderWholeMap(false)
	, m_currHoveredTile(nullptr)
	, m_currPathFind(nullptr)
	, m_playerJustDied(false)
	, m_exiting(false)
	, m_fadeAge(0.f)
{
	m_squirrelFont = BitmapFont::CreateOrGetFont("SquirrelFixedFont");
}
TheWorld::~TheWorld() { }

//---------------------------------------------------------------------------------------------------------------------------
//INIT & CLEANUP
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::InitializeEntities() {

	m_thePlayer = nullptr;
	if (m_entitiesOnMap.size() != 1) {
		for (unsigned int i = 0; i < m_entitiesOnMap.size(); i++) {
			if (m_entitiesOnMap[i])
				delete m_entitiesOnMap[i];
		}
	}

	m_thePlayer = new Player('@', RGBA::WHITE, RGBA::BLACK, m_activeMap);
	DeleteCurrPathFinder();

	m_entitiesOnMap.push_back(m_thePlayer);
}
void TheWorld::InitializeEntitiesForLoadedGame() {

	m_thePlayer = nullptr;
	for (unsigned int i = 0; i < m_entitiesOnMap.size(); i++) {
		delete m_entitiesOnMap[i];
	}
	m_entitiesOnMap.clear();

	m_fadeAge = 0.f;
	m_playerJustDied = false;
	m_exiting = false;
	m_currHoveredTile = nullptr;
	m_playerJustDied = false;
	m_exiting = false;
	m_fadeAge = 0.f;
	m_age = 0.f;
	m_renderWholeMap = false;

	DeleteCurrPathFinder();

	m_playerPathFind = nullptr;
	m_currPathFind = nullptr;
}
void TheWorld::FlushActiveEntities() {

	m_entitiesOnMap.clear();

	m_fadeAge = 0.f;
	m_playerJustDied = false;
	m_exiting = false;
}
void TheWorld::CleanUpDeadEntities() {

	std::vector<Entity*>::iterator entityIter = m_entitiesOnMap.begin();
	while (entityIter != m_entitiesOnMap.end()) {
		Entity* currEntity = *entityIter;
		Agent* currAgent = nullptr;

		if (currEntity->IsAgent() && !currEntity->IsPlayer() && !dynamic_cast<Agent*>(currEntity)->IsAlive()) {

			currAgent = dynamic_cast<Agent*>(currEntity);
			currAgent->LogDeath();

			delete *entityIter;
			entityIter = m_entitiesOnMap.erase(entityIter);
			continue;
		}
		else if (currEntity->IsPlayer() && !dynamic_cast<Agent*>(currEntity)->IsAlive()) {
			KillPlayer();
			entityIter = m_entitiesOnMap.erase(entityIter);

			break;
		}

		entityIter++;
	}

}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::Update(float deltaSeconds) {
	UNREFERENCED_PARAMETER(deltaSeconds);

		UpdateDeathAnim(deltaSeconds);

	if (nullptr != m_thePlayer) {
		m_thePlayer->Update(deltaSeconds);
	}

	if (g_theInputSystem->GetKeyDown('M')) {
		m_renderWholeMap = !m_renderWholeMap;
	}

	if (m_activeMap) {
		m_activeMap->Update(deltaSeconds);
	}

	g_theCombatSystem->UpdateTurnOrder(deltaSeconds);

	//For of war stuff
	//Set stuff on screen back to not visible
	IntVector2 mapSize = m_activeMap->GetSize();
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {

			//Set up rendering box
			IntVector2 position = IntVector2(x, y);
			IntVector2 numTilesOnScreen = m_activeMap->GetSize();
			Vector2 cellSize = Vector2(SCREEN_SIZE.x / (float)numTilesOnScreen.x, SCREEN_SIZE.y / (float)numTilesOnScreen.y);

			Vector2 positionToDrawFrom = position.ToVector2() * cellSize;
			Vector2 positionToDrawTo = positionToDrawFrom + cellSize;
			m_activeMap->GetTileAtLocation(position)->m_renderingBox = AABB2(positionToDrawFrom, positionToDrawTo);

			if (!m_renderWholeMap) {
				m_activeMap->GetTileAtLocation(IntVector2(x, y))->SetVisibility(false);
			}
			else {
				m_activeMap->GetTileAtLocation(IntVector2(x, y))->SetVisibility(true);
			}
		}
	}

	if (!g_theWorld->m_thePlayer)
		return;

	if (!m_renderWholeMap) {
		UpdateFogOfWar();
	}

	UpdateHoveringTileAndPathfindToIt();

	UpdateTilePointers();

	for (unsigned int i = 0; i < m_entitiesOnMap.size(); i++) {
		m_entitiesOnMap[i]->Update(deltaSeconds);
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::UpdateTilePointers() {

	IntVector2 mapSize = m_activeMap->GetSize();

	//Clear pointers on map
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			m_activeMap->GetTileAtLocation(IntVector2(x, y))->SetOccupyingAgent(nullptr);
		}
	}
	for (unsigned int i = 0; i < m_entitiesOnMap.size(); i++) {
		if (m_entitiesOnMap[i]->IsAgent()) {
			m_activeMap->GetTileAtLocation(m_entitiesOnMap[i]->GetPosition())->SetOccupyingAgent(dynamic_cast<Agent*>(m_entitiesOnMap[i]));
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::UpdateHoveringTileAndPathfindToIt() {
	IntVector2 mapSize = m_activeMap->GetSize();
	Vector2 relMousePos = g_theInputSystem->GetRelativeCursorPosition().ToVector2();

	m_currHoveredTile = nullptr;

	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			IntVector2 currLocation = IntVector2(x, y);
			Tile* currTile = m_activeMap->GetTileAtLocation(currLocation);

			if (relMousePos.x > currTile->m_renderingBox.mins.x
				&& relMousePos.y > currTile->m_renderingBox.mins.y
				&& relMousePos.x < currTile->m_renderingBox.maxs.x
				&& relMousePos.y < currTile->m_renderingBox.maxs.y
				&& currTile->GetCurrentTileType() != TILE_AIR)
			{
				currTile->m_isBeingHoveredOn = true;
				m_currHoveredTile = currTile;

			}
			else {
				currTile->m_isBeingHoveredOn = false;
			}
		}
	}

	if (!m_currHoveredTile) {
		m_thePlayer->m_playerPathFind->m_currBestPath.clear();
	}

	if (DEBUG_RENDER_PATH_FIND) {
		if (m_currHoveredTile && m_thePlayer&& m_currHoveredTile->GetCurrentTileType() != TILE_STONE) {
			
			m_thePlayer->m_playerPathFind->FindBestPathFromStartToGoal(m_thePlayer->GetPosition(), m_currHoveredTile->GetLocation(), PLAYER_MOVEMENT_PROPERTIES);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//DEATH ANIM
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::UpdateDeathAnim(float deltaSeconds) {
	if (m_playerJustDied) {
		m_fadeAge += deltaSeconds / 10.f;

		if (m_fadeAge >= 1.f) {
			m_exiting = true;
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::RunNPCs() {
	for (unsigned int i = 0; i < m_entitiesOnMap.size(); i++) {
		m_entitiesOnMap[i]->Run();
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::Render() const {
	RGBA bgColor = RGBA(0x00, 0x00, 0x00, 0xFF);
	g_theRenderer->ClearScreen(bgColor.r, bgColor.g, bgColor.b);

	if (m_activeMap != nullptr) {
		IntVector2 mapSize = m_activeMap->GetSize();

		for (int x = 0; x < mapSize.x; x++) {
			for (int y = 0; y < mapSize.y; y++) {
				IntVector2 currLocation = IntVector2(x, y);
				Tile* currTile = m_activeMap->GetTileAtLocation(currLocation);

				RenderTile(currTile, currLocation);
			}
		}
	}

	RenderPathFind();

	RenderVerts();

	if (nullptr != m_thePlayer) {
		m_thePlayer->Render();
	}

	RenderItemsOnMap();

	RenderEntities();

	g_theRenderer->DrawAABB2(AABB2(Vector2(0.f, 0.f), Vector2(1600.f, 900.f)), RGBA(0.f, 0.f, 0.f, m_fadeAge), PRIMITIVE_QUADS);
}

//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::RenderTile(Tile* tile, const IntVector2& position) const {
	IntVector2 numTilesOnScreen = m_activeMap->GetSize();
	Vector2 cellSize = Vector2(SCREEN_SIZE.x / (float)numTilesOnScreen.x, SCREEN_SIZE.y / (float)numTilesOnScreen.y);

	Vector2 positionToDrawFrom = position.ToVector2() * cellSize;
	Vector2 positionToDrawTo = positionToDrawFrom + cellSize;

	char glyphToDraw = tile->GetGlyph();

	//Get color
	RGBA color = tile->GetColor();
	RGBA bgColor = (tile->IsDynamicallyRendered()) ? (tile->GetDynamicColor()) : (tile->GetBackgroundColor());

	if (tile->m_isBeingHoveredOn) {
		color = RGBA(1.f, 1.f, 1.f, 0.7f);
	}

	float alpha = 0.f;

	//Explore it if the bressenham lines
	if (tile->IsVisible() == false) {
		if (tile->IsExplored() && !tile->IsVisible()) {
			alpha = 0.7f;
		}
		else if (tile->IsExplored() && tile->IsVisible()) {
			alpha = 1.f;
		}
		else if (!tile->IsExplored() && !tile->IsVisible()) {
			alpha = 0.f;
		}

		color.a = color.a * alpha;
		bgColor.a = bgColor.a * alpha;
	}

	AddAABB2Verts(AABB2(positionToDrawFrom, positionToDrawTo), bgColor);
	AddAABB2Text(AABB2(positionToDrawFrom, positionToDrawTo - Vector2(0.5f, 0.5f)), color, glyphToDraw);
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::RenderItemsOnMap() const {
	IntVector2 mapSize = m_activeMap->GetSize();

	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			Tile* currTile = m_activeMap->GetTileAtLocation(IntVector2(x, y));

			if (currTile->IsVisible() && currTile->GetInventory()) {
				currTile->GetInventory()->Render(m_activeMap, currTile->GetLocation());
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::RenderEntities() const {

	if (m_thePlayer) {
		for (unsigned int i = 0; i < m_entitiesOnMap.size(); i++) {
			if ((m_thePlayer->CanSeeEntity(m_entitiesOnMap[i]) || m_renderWholeMap) && !m_entitiesOnMap[i]->IsItem()) {
				m_entitiesOnMap[i]->Render();
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::RenderPathFind() const {
	if (nullptr != m_currPathFind) {
		DebugRenderPathNode(m_currPathFind->m_goal, RGBA(0.f, 1.f, 0.f, 0.5f));

		//BLUE
		for (std::list<PathNode*>::iterator it = m_currPathFind->m_openList.begin(); it != m_currPathFind->m_openList.end(); ++it) {
			PathNode* currNode = *it;
			DebugRenderPathNode(currNode->m_position, RGBA(0.f, 0.f, 1.f, 0.5f));
		}

		//RED
		for (std::list<PathNode*>::iterator it = m_currPathFind->m_closedList.begin(); it != m_currPathFind->m_closedList.end(); ++it) {
			PathNode* currNode = *it;
			DebugRenderPathNode(currNode->m_position, RGBA(1.f, 0.f, 0.f, 0.5f));
		}
	}

	if (m_thePlayer && m_thePlayer->m_playerPathFind) {
		for (unsigned int i = 0; i < m_thePlayer->m_playerPathFind->m_currBestPath.size(); i++) {
			DebugRenderPathNode(m_thePlayer->m_playerPathFind->m_currBestPath[i], RGBA(1.f, 1.f, 1.f, 0.3f));
		}
	}

	if (m_currHoveredTile) {
		DebugRenderPathNode(m_currHoveredTile->GetLocation(), RGBA(1.f, 1.f, 1.f, 0.5f));
	}

	if (DEBUG_RENDER_PATH_FIND) {
		if (m_thePlayer && m_thePlayer->m_playerPathFind) {
			for (unsigned int i = 0; i < m_thePlayer->m_playerPathFind->m_currBestPath.size(); i++) {
				DebugRenderPathNode(m_thePlayer->m_playerPathFind->m_currBestPath[i], RGBA(1.f, 1.f, 1.f, 0.3f));
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//PATHFIND
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::DeleteCurrPathFinder() {
	if (nullptr != m_currPathFind) {
		delete m_currPathFind;
		m_currPathFind = nullptr;
	}
}
void TheWorld::CreateOrStepPathFind() {
	if (nullptr != m_currPathFind && !m_currPathFind->m_isFinished) {
		m_currPathFind->RunOneStep(0);
	}
	else {
		IntVector2 goal = m_activeMap->FindRandomValidLocation();
		m_currPathFind = new PathFinder(m_thePlayer->GetPosition(), goal);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE MEMBER FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::DebugRenderPathNode(const IntVector2& position, const RGBA& color) const {
	IntVector2 numTilesOnScreen = m_activeMap->GetSize();
	Vector2 cellSize = Vector2(SCREEN_SIZE.x / (float)numTilesOnScreen.x, SCREEN_SIZE.y / (float)numTilesOnScreen.y);

	Vector2 positionToDrawFrom = position.ToVector2() * cellSize;
	Vector2 positionToDrawTo = positionToDrawFrom + cellSize;

	//g_theRenderer->DrawAABB2(AABB2(positionToDrawFrom, positionToDrawTo), color, PRIMITIVE_QUADS);
	AddAABB2Verts(AABB2(positionToDrawFrom, positionToDrawTo), color);

}

//---------------------------------------------------------------------------------------------------------------------------
//FOG OF WAR
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::UpdateFogOfWar() {

	if (!g_theWorld->m_thePlayer) {
		return;
	}

	m_thePlayer->m_visibleEntities.clear();

	IntVector2 heroLoc = m_thePlayer->GetPosition();
	vector<IntVector2> circPoints;

	
	//Find the circumference of the circle around the hero
	for (int radius = 5; radius < 26; radius ++) {
		IntVector2 prevPoint = IntVector2(0, 0);
		for (double rad = 0; rad < pi / 4; rad += 0.04) {
			double xTrig = (double)radius * cos(rad);
			double yTrig = (double)radius * sin(rad);
			int x = (int)((double)heroLoc.x + xTrig);
			int y = (int)((double)heroLoc.y + yTrig);
			IntVector2 p(x, y);
			if (p != prevPoint) {
				circPoints.push_back(p);
				prevPoint = p;
			}
		}
	}

	//Go through each octant and draw the bressenham lines
	for (int octant = 0; octant < 8; octant++) {
		for (unsigned int i = 0; i < circPoints.size(); i++) {
			DrawBressenhamLine(octant, circPoints[i]);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::DrawBressenhamLine(int octant, IntVector2 delta) {
	IntVector2 relLoc = IntVector2(0, 0);
	IntVector2 start = m_thePlayer->GetPosition();
	IntVector2 end = delta;
	delta = end - start;

	int D = 2 * delta.y - delta.x;
	int y = start.y;

	IntVector2 heroLoc = m_thePlayer->GetPosition();

	m_activeMap->GetTileAtLocation(heroLoc)->Explore();
	m_activeMap->GetTileAtLocation(heroLoc)->SetVisibility(true);

	for (int x = start.x + 1; x < end.x; x++) {
		IntVector2 rel;
		if (D > 0) {
			y += 1;
			rel = relLoc + SwitchOctant(octant, IntVector2(x, y));

			Tile* currTile = m_activeMap->GetTileAtLocation(rel);

			if (currTile && currTile->IsBlockingLOS()) {
				currTile->Explore();
				currTile->SetVisibility(true);
				return;
			}
			else if(currTile) {
				currTile->Explore();
				currTile->SetVisibility(true);

				if (currTile->GetOccupyingAgent()) {
					m_thePlayer->m_visibleEntities.push_back(currTile->GetOccupyingAgent());
				}
				if (currTile->GetInventory()) {
					Inventory* inv = currTile->GetInventory();
					for (unsigned int i = 0; i < inv->GetItemsInBag()->size(); i++) {
						m_thePlayer->m_visibleEntities.push_back(inv->GetItemsInBag()->at(i));
					}
				}
			}

			D += (2 * delta.y) - (2 * delta.x);
		}
		else {
			rel = relLoc + SwitchOctant(octant, IntVector2(x, y));

			Tile* currTile = m_activeMap->GetTileAtLocation(rel);

			if (currTile && currTile->IsBlockingLOS()) {
				currTile->Explore();
				currTile->SetVisibility(true);
				return;
			}
			else if(currTile) {
				currTile->Explore();
				currTile->SetVisibility(true);

				if (currTile->GetOccupyingAgent()) {
					m_thePlayer->m_visibleEntities.push_back(m_activeMap->GetTileAtLocation(rel)->GetOccupyingAgent());
				}
				if (currTile->GetInventory()) {
					Inventory* inv = currTile->GetInventory();
					for (unsigned int i = 0; i < inv->GetItemsInBag()->size(); i++) {
						m_thePlayer->m_visibleEntities.push_back(inv->GetItemsInBag()->at(i));
					}
				}
			}

			D += (2 * delta.y);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
IntVector2 TheWorld::SwitchOctant(int octant, IntVector2 p) {
	IntVector2 heroLoc = m_thePlayer->GetPosition();

	switch (octant) {
	case 0: {
		return IntVector2(p.x, p.y);
	}
	case 1: {
		p = p - heroLoc;
		IntVector2 ret = IntVector2(p.y, p.x);
		ret = ret + heroLoc;
		return ret;
	}
	case 2: {
		p = p - heroLoc;
		IntVector2 ret(p.y, p.x);
		ret = ret + heroLoc;
		ret.y = ret.y - 2 * (ret.y - heroLoc.y);
		return ret;
	}
	case 3: {
		IntVector2 ret = p;
		ret.x = ret.x - 2 * (ret.x - heroLoc.x);
		return ret;
	}
	case 4: {
		IntVector2 ret = p;
		ret.x = ret.x - 2 * (ret.x - heroLoc.x);
		ret.y = ret.y - 2 * (ret.y - heroLoc.y);
		return ret;
	}
	case 5: {
		p = p - heroLoc;
		IntVector2 ret(p.y, p.x);
		ret = ret + heroLoc;
		ret.x = ret.x - 2 * (ret.x - heroLoc.x);
		ret.y = ret.y - 2 * (ret.y - heroLoc.y);
		return ret;
	}
	case 6: {
		p = p - heroLoc;
		IntVector2 ret(p.y, p.x);
		ret = ret + heroLoc;
		ret.x = ret.x - 2 * (ret.x - heroLoc.x);
		return ret;
	}
	case 7: {
		IntVector2 ret = p;
		ret.y = ret.y - 2 * (ret.y - heroLoc.y);
		return ret;
	}
	default: {
		return IntVector2(0, 0);
	}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//BATCH DRAWING
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::RenderVerts() const {
	g_theRenderer->DisableTextures();
	g_theRenderer->DrawVertexArray(s_renderVerts, s_renderVerts.size(), PRIMITIVE_QUADS);

	g_theRenderer->BindAndEnableTexture(const_cast<Texture*>(m_squirrelFont->GetSpriteSheet()->GetTexture()));
	g_theRenderer->DrawVertexArray(s_tRenderVerts, s_tRenderVerts.size(), PRIMITIVE_QUADS);

	s_renderVerts.clear();
	s_tRenderVerts.clear();
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::AddAABB2Verts(const AABB2& quad, const RGBA& color) const {
	s_renderVerts.push_back(Vertex(Vector3(quad.mins.x, quad.mins.y, 0.f), color));
	s_renderVerts.push_back(Vertex(Vector3(quad.maxs.x, quad.mins.y, 0.f), color));
	s_renderVerts.push_back(Vertex(Vector3(quad.maxs.x, quad.maxs.y, 0.f), color));
	s_renderVerts.push_back(Vertex(Vector3(quad.mins.x, quad.maxs.y, 0.f), color));
}
//---------------------------------------------------------------------------------------------------------------------------
void TheWorld::AddAABB2Text(const AABB2& quad, const RGBA& color, char charToDraw) const {

	AABB2 texCoords = m_squirrelFont->GetTexCoordsForGlyph(charToDraw);

	s_tRenderVerts.push_back(TexturedVertex(Vector3(quad.mins.x, quad.mins.y, 0.f), color, Vector2(texCoords.mins.x, texCoords.maxs.y)));
	s_tRenderVerts.push_back(TexturedVertex(Vector3(quad.maxs.x, quad.mins.y, 0.f), color, Vector2(texCoords.maxs.x, texCoords.maxs.y)));
	s_tRenderVerts.push_back(TexturedVertex(Vector3(quad.maxs.x, quad.maxs.y, 0.f), color, Vector2(texCoords.maxs.x, texCoords.mins.y)));
	s_tRenderVerts.push_back(TexturedVertex(Vector3(quad.mins.x, quad.maxs.y, 0.f), color, Vector2(texCoords.mins.x, texCoords.mins.y)));
}

void TheWorld::KillPlayer() {
	g_theGame->m_numMonstersKilled = g_theCombatSystem->m_numMonstersKilled;
	g_theGame->m_turnCountPlayerDied = g_theCombatSystem->GetTurnCount();

	m_playerJustDied = true;
}

void TheWorld::AddItemToEntityList(Item* item) {
	m_entitiesOnMap.push_back(item);
}

//---------------------------------------------------------------------------------------------------------------------------
//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
uint TheWorld::GetEntityIDFromSavedID(uint savedID) {

	uint id = 0;
	bool foundOne = false;
	for (unsigned int i = 0; i < m_entitiesOnMap.size(); i++) {
		if (m_entitiesOnMap[i]->IsAgent()) {
			Agent* currAgent = static_cast<Agent*>(m_entitiesOnMap[i]);

			if (savedID == currAgent->GetSavedID()) {

				id = currAgent->GetEntityID();
				foundOne = true;
			}
		}
	}

	return id;
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVE LOADING
//---------------------------------------------------------------------------------------------------------------------------
XMLNode TheWorld::SaveEntityData() const {
	
	ASSERT_OR_DIE(m_thePlayer != nullptr, "ERROR: Player should not be null when saving");

	XMLNode entityData;
	entityData = entityData.createXMLTopNode("EntityData");

	for (unsigned int i = 0; i < m_entitiesOnMap.size(); i++) {
		XMLNode entityNode = m_entitiesOnMap[i]->SaveToXML();
		entityData.addChild(entityNode);
	}

	IntVector2 mapSize = m_activeMap->GetSize();
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			IntVector2 loc = IntVector2(x, y);

			Tile* currTile = m_activeMap->GetTileAtLocation(loc);
			if (currTile->GetInventory()) {
				XMLNode invNode = currTile->GetInventory()->SaveToXML(loc);
				entityData.addChild(invNode);
			}
		}
	}

	return entityData;
}