#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/RangeInt.hpp"

extern const IntVector2 CELL_SIZE; // = IntVector2(20, 20);
const int NUM_TILES_WIDTH = 170;
const int NUM_TILES_HEIGHT = 100;

const int NUM_ENEMIES_ON_MAP = 20;

extern const bool DEBUG_DRAW_TILES;

const IntVector2 NORTH     = IntVector2( 0,  1);
const IntVector2 NORTHEAST = IntVector2( 1,  1);
const IntVector2 EAST      = IntVector2( 1,  0);
const IntVector2 SOUTHEAST = IntVector2( 1, -1);
const IntVector2 SOUTH     = IntVector2( 0, -1);
const IntVector2 SOUTHWEST = IntVector2(-1, -1);
const IntVector2 WEST      = IntVector2(-1,  0);
const IntVector2 NORTHWEST = IntVector2(-1,  1);

const Vector2 SCREEN_SIZE = Vector2(1400.f, 775.f);

//MESSAGE LOG
const int MAX_MESSAGES_MESSAGE_LOG = 5;
const int MAX_NEW_MESSAGES_MESSAGE_LOG = 2;
const Vector2 MESSAGE_LOG_START_LOC = Vector2(210.f, 775.f);

//INFO LOG
const Vector2 INFO_LOG_START_LOC = Vector2(10.f, 890.f);
const Vector2 INFO_LOG_END_LOC = Vector2(200.f, 750.f);

//NPC
const String NPC_DEFAULT_NAME = "NULL";
const char NPC_ITEM_DEFAULT_GLYPH = '?';
extern const RGBA NPC_DEFAULT_COLOR;
extern const RangeInt NPC_DEFAULT_HEALTH;
const float NPC_DEFAULT_MOVE_SPEED = 1.f;
extern const RangeInt NPC_DEFAULT_DAMAGE;

//ITEM
const int NUM_ITEMS_ON_MAP = 20;
const String ITEM_DEFAULT_NAME = "NULLITEM";
const int MAX_NUM_ITEMS_INVENTORY = 20;
const int NPC_DEFAULT_ARMOR = 0;
extern const RGBA HIGHLIGHT_COLOR;
extern const RGBA HIGHLIGHT_DOWN_COLOR;

extern bool DEBUG_RENDER_PATH_FIND;

//FEATURES
const String FEATURE_DEFAULT_NAME = "FEATURE";

//TILE DEFINITIONS
static const char TILE_AIR_GLYPH = ' ';
static const RGBA TILE_AIR_COLOR = RGBA::BLACK;
static const RGBA TILE_AIR_BGCOLOR = RGBA::BLACK;

static const char TILE_STONE_GLYPH = '%';
static const RGBA TILE_STONE_COLOR = RGBA::ORANGE;
static const RGBA TILE_STONE_BGCOLOR = RGBA::BLACK;

static const char TILE_GRASS_GLYPH = '.';
static const RGBA TILE_GRASS_COLOR = RGBA(0.5f, 0.5f, 0.5f, 0.5f);
static const RGBA TILE_GRASS_BGCOLOR = RGBA(0x00, 0x01, 0x10, 0xFF);

static const char TILE_WATER_GLYPH = '$';
static const RGBA TILE_WATER_COLOR = RGBA::BLUE;
static const RGBA TILE_WATER_BGCOLOR = RGBA::BLACK;

static const char TILE_LAVA_GLYPH = 'x';
static const RGBA TILE_LAVA_COLOR = RGBA::RED;
static const RGBA TILE_LAVA_BGCOLOR = RGBA::ORANGE;