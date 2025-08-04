#ifndef SOKO_H
#define SOKO_H

// ----Includes--------------
#include "../../glyph/glyph.h"

// (Experimental)
// ----Tile------------------
typedef struct {
  chtype ch;
  int color;
  bool blocks_movement;
  bool movable;
} tile_t;

// Tile Templates
extern tile_t tile_empty;
extern tile_t tile_wall;
extern tile_t tile_rock;
extern tile_t tile_hole;

// (Experimental)
// ----Map-------------------
extern int MAP_HEIGHT;
extern int MAP_WIDTH;

extern tile_t **gp_map;

tile_t **map_create(view_data_t *p_view);
// Test
void test_level_init();
void test_map_init();
void map_draw(view_data_t *p_view, tile_t **p_map);
void map_free();

// ----Sokoban---------------
// ----Game-Scene------------
extern scene_data_t *gp_soko_game_scene;

void soko_game_init();
void soko_game_handle_input();
void soko_game_update();
void soko_game_draw();
void soko_game_shutdown();

#endif // !SOKO_H
