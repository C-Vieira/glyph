#ifndef SNAKE_H
#define SNAKE_H

// ----Includes--------------
#include "../../glyph/glyph.h"

// ----Snake-Game------------
// ----Game-Scene------------
extern scene_data_t *gp_snake_title_scene;

void snake_title_init();
void snake_title_handle_input();
void snake_title_update();
void snake_title_draw();
void snake_title_shutdown();

// ----Title-Scene-----------
extern scene_data_t *gp_snake_game_scene;

void snake_game_init();
void snake_game_handle_input();
void snake_game_update();
void snake_game_draw();
void snake_game_shutdown();

// ----Player----------------
typedef struct snake_player {
  entity_t player;
  queue_t *p_segments;
} snake_player_t;

// Global player reference (could just be static)
extern snake_player_t g_player;

entity_t *snake_get_head(snake_player_t *p_snake);
entity_t *snake_get_tail(snake_player_t *p_snake);
void snake_add_segment(snake_player_t *p_snake, vec2_t new_pos);
void snake_remove_segment(snake_player_t *p_snake);
void snake_move_to(snake_player_t *p_snake, vec2_t new_pos);

#endif // !SNAKE_H
