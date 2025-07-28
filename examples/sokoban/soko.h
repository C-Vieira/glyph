#ifndef SOKO_H
#define SOKO_H

// ----Includes--------------
#include "../../glyph/glyph.h"

// ----Sokoban---------------
// ----Game-Scene------------
extern scene_data_t *gp_soko_game_scene;

void soko_game_init();
void soko_game_handle_input();
void soko_game_update();
void soko_game_draw();
void soko_game_shutdown();

#endif // !SOKO_H
