#ifndef CCRAWL_H
#define CCRAWL_H

#include "../../glyph/glyph.h"

// ----Game-Scene------------
extern scene_data_t *gp_ccrawl_game_scene;

void ccrawl_game_init();
void ccrawl_game_handle_input();
void ccrawl_game_update();
void ccrawl_game_draw();
void ccrawl_game_shutdown();

#endif // !CCRAWL_H
