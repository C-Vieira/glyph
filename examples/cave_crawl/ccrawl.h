#ifndef CCRAWL_H
#define CCRAWL_H

#include "../../glyph/glyph.h"

// ----Experimental----------
// ----Generator-------------
typedef enum { RAND_ROOMS, CA_CAVES } generator_type_e;

vec2_t generator_generate_level(generator_type_e type, tile_map_t *p_map);

// ----Game-Scene------------
extern scene_data_t *gp_ccrawl_game_scene;

void ccrawl_game_init();
void ccrawl_game_handle_input();
void ccrawl_game_update();
void ccrawl_game_draw();
void ccrawl_game_shutdown();

#endif // !CCRAWL_H
