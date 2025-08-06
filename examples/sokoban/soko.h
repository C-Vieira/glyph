#ifndef SOKO_H
#define SOKO_H

// ----Includes--------------
#include "../../glyph/glyph.h"

// ----Tile------------------
// Tile Ids
#define TILE_EMPTY 0
#define TILE_WALL 1
#define TILE_ROCK 2
#define TILE_HOLE 3
#define TILE_FILLED_HOLE 4

// Tile Templates
extern tile_t tile_empty;
extern tile_t tile_wall;
extern tile_t tile_rock;
extern tile_t tile_hole;
extern tile_t tile_filled_hole;

// ----Map-------------------
// Hole tile positions
extern dyn_array_t g_hole_positions;

// Test
void test_level_init(tile_map_t map);
void test_map_init(tile_map_t map_ground, tile_map_t map_surface);

// ----Sokoban---------------
// ----Game-Scene------------
extern scene_data_t *gp_soko_game_scene;

void soko_game_init();
void soko_game_handle_input();
void soko_game_update();
void soko_game_draw();
void soko_game_shutdown();

#endif // !SOKO_H
