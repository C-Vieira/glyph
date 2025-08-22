#ifndef SOKO_H
#define SOKO_H

// ----Includes--------------
#include "../../glyph/glyph.h"
#include <string.h>

// ----Tile------------------
// Tile Ids
#define TILE_EMPTY 0
#define TILE_WALL 1
#define TILE_ROCK 2
#define TILE_HOLE 3
#define TILE_FILLED_HOLE 4
#define TILE_CONVEYOR_LEFT 5
#define TILE_CONVEYOR_RIGHT 6
#define TILE_CONVEYOR_DOWN 7
#define TILE_LETTER_W 8
#define TILE_LETTER_I 9
#define TILE_LETTER_N 10
#define TILE_PLAYER_SPAWN 11

// Tile Templates
extern tile_t tile_empty;
extern tile_t tile_wall;
extern tile_t tile_rock;
extern tile_t tile_hole;
extern tile_t tile_filled_hole;
extern tile_t tile_conveyor_left;
extern tile_t tile_conveyor_right;

// ----Level-----------------
// Level Ids
#define LEVEL_EASY 0
#define LEVEL_MEDIUM 1
#define LEVEL_HARD 2
#define LEVEL_TEST 100

// Level state
// Current level
extern int curr_level;
// Hole tile positions
extern dyn_array_t g_hole_positions;
// Conveyor tile positions
extern dyn_array_t g_conveyor_positions;
// W tile position
extern vec2_t g_w_tile_pos;

vec2_t soko_level_init(int level_id, tile_map_t map_ground,
                       tile_map_t map_surface);

// ----Sokoban---------------
// ----Title-Scene-----------
extern scene_data_t *gp_soko_title_scene;

void soko_title_init();
void soko_title_handle_input();
void soko_title_update();
void soko_title_draw();
void soko_title_shutdown();

// ----LevelSelect-Scene-----
extern scene_data_t *gp_soko_lvlselect_scene;

void soko_lvlselect_init();
void soko_lvlselect_handle_input();
void soko_lvlselect_update();
void soko_lvlselect_draw();
void soko_lvlselect_shutdown();

// ----Game-Scene------------
extern scene_data_t *gp_soko_game_scene;

void soko_game_init();
void soko_game_handle_input();
void soko_game_update();
void soko_game_draw();
void soko_game_shutdown();

#endif // !SOKO_H
