#include "../ccrawl.h"
#include <curses.h>

scene_data_t *gp_ccrawl_game_scene;

// Player reference
static entity_t s_player;

// Views
static view_data_t *sp_game_view;

void ccrawl_game_init() {
  // Game view init
  sp_game_view = view_create(28, 50, 0, 0);
  view_add_border(sp_game_view);

  // Player Init
  vec2_t start_pos = {.x = 5, .y = 5};
  s_player =
      (entity_t){.pos = start_pos, .ch = '@', .color = COLOR_PAIR(GREEN_BLACK)};
  // Draw it on the screen
  view_draw_entity(sp_game_view, &s_player);

  view_draw(sp_game_view);
}

void ccrawl_game_handle_input() {
  chtype input = view_get_input(sp_game_view);
  if (input == 'q') {
    g_game.should_close = true;
  }

  vec2_t new_dir;
  switch (input) {
  case 'w': {
    new_dir = VEC_UP;
    break;
  }
  case 'a': {
    new_dir = VEC_LEFT;
    break;
  }
  case 's': {
    new_dir = VEC_DOWN;
    break;
  }
  case 'd': {
    new_dir = VEC_RIGHT;
    break;
  }
  default:
    new_dir = VEC_ZERO;
  }

  // Set player direction
  s_player.dir = new_dir;
}

void ccrawl_game_update() {
  // Calculate new player position
  vec2_t new_pos = vector_add(s_player.pos, s_player.dir);
  view_clear_char_at(sp_game_view, s_player.pos);

  // Handle collisions

  // Update player position
  s_player.pos = new_pos;
}

void ccrawl_game_draw() {
  // Draw player
  view_draw_entity(sp_game_view, &s_player);

  view_draw(sp_game_view);
}

void ccrawl_game_shutdown() {
  // Free view windows
  delwin(sp_game_view->p_view_window);

  free(sp_game_view);
}
