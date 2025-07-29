#include "../soko.h"

scene_data_t *gp_soko_game_scene;

// Player reference
static entity_t s_player;

// Test rock entity
static entity_t s_rock;

// Views
static view_data_t *p_game_view;

// Map
tile_t **gp_map;

void soko_game_init() {
  // View Init
  p_game_view = view_create(28, 50, 0, 0);
  view_add_border(p_game_view);

  // Map Init
  gp_map = map_create(p_game_view);
  map_test_init();
  // Draw map
  map_draw(p_game_view, gp_map);

  // Player Init
  s_player =
      (entity_t){.pos = {3, 8}, .ch = '@', .color = COLOR_PAIR(GREEN_BLACK)};
  // Draw it on the screen
  view_draw_entity_at(p_game_view, &s_player);

  // Test rock entity
  s_rock =
      (entity_t){.pos = {5, 12}, .ch = 'O', .color = COLOR_PAIR(YELLOW_BLACK)};
  // Draw it on the screen
  view_draw_entity_at(p_game_view, &s_rock);

  view_draw(p_game_view);
}

void soko_game_handle_input() {
  chtype input = view_get_input(p_game_view);
  if (input == 'q')
    g_game.should_close = true;

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
    new_dir = s_player.dir;
  }

  // Set player direction
  s_player.dir = new_dir;
}

void soko_game_update() {
  vec2_t new_pos = vector_add(s_player.dir, s_player.pos);

  bool player_is_colliding =
      game_is_colliding_with_border(p_game_view, new_pos) ||
      !gp_map[new_pos.y][new_pos.x].walkable;
  bool rock_is_colliding = false;

  // Check for rock
  if (view_get_char_at(p_game_view, new_pos.y, new_pos.x) ==
      ('O' | COLOR_PAIR(YELLOW_BLACK))) {
    vec2_t rock_new_pos = vector_add(s_player.dir, s_rock.pos);

    rock_is_colliding =
        game_is_colliding_with_border(p_game_view, rock_new_pos) ||
        !gp_map[rock_new_pos.y][rock_new_pos.x].walkable;
    if (!rock_is_colliding) {
      // Update rock position
      s_rock.pos = rock_new_pos;
    }
  }

  // Check collisions
  if (!player_is_colliding && !rock_is_colliding) {

    // Clear trail
    vec2_t old_pos = s_player.pos;
    view_clear_char_at(p_game_view, old_pos.y, old_pos.x);

    // Update player position
    s_player.pos = new_pos;
  }
}

void soko_game_draw() {
  // Draw player
  view_draw_entity_at(p_game_view, &s_player);

  // Draw rock
  view_draw_entity_at(p_game_view, &s_rock);

  view_draw(p_game_view);
}

void soko_game_shutdown() {
  // Free map
  map_free();

  delwin(p_game_view->p_view_window);

  free(p_game_view);
}
