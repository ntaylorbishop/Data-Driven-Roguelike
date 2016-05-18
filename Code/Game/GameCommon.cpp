#include "Game/GameCommon.hpp"

const IntVector2 CELL_SIZE = IntVector2(20, 20);
const bool DEBUG_DRAW_TILES = false;

const RGBA NPC_DEFAULT_COLOR = RGBA::MAGENTA;
const RangeInt NPC_DEFAULT_HEALTH = RangeInt(10, 10);
const RangeInt NPC_DEFAULT_DAMAGE = RangeInt(2, 3);

const RGBA HIGHLIGHT_COLOR = RGBA(0.f, 1.f, 1.f, 0.4f);
const RGBA HIGHLIGHT_DOWN_COLOR = RGBA(0.f, 1.f, 1.f, 0.1f);

bool DEBUG_RENDER_PATH_FIND = true;

//---------------------------------------------------------------------------------------------------------------------------
//TILE DEFINITIONS
//---------------------------------------------------------------------------------------------------------------------------

