#ifndef CCRAWL_H
#define CCRAWL_H

#include "../../glyph/glyph.h"

// Tile Templates
extern tile_t tile_empty;
extern tile_t tile_wall;
extern tile_t tile_exit;

// ----Experimental----------
// ----BSP-------------------
#define NUM_PARTITIONS 6
#define MIN_ROOM_SIZE 6

typedef struct bsp_node_t {
  vec2_t node_pos;
  int node_height;
  int node_width;

  struct bsp_node_t *left;
  struct bsp_node_t *right;
} bsp_node_t;

// ----Generator-------------
typedef enum { RAND_ROOMS, CA_CAVES } generator_type_e;

void generator_generate_level(generator_type_e type, tile_map_t *p_map);

// ----Game-Scene------------
extern scene_data_t *gp_ccrawl_game_scene;

void ccrawl_game_init();
void ccrawl_game_handle_input();
void ccrawl_game_update();
void ccrawl_game_draw();
void ccrawl_game_shutdown();

#endif // !CCRAWL_H
