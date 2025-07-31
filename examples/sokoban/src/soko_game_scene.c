#include "../soko.h"

scene_data_t *gp_soko_game_scene;

// Player reference
static entity_t s_player;

// Views
static view_data_t *p_game_view;

// Map
tile_t **gp_map;

void soko_game_init() {
  // View Init
  // p_game_view = view_create(28, 50, 0, 0);
  p_game_view = view_create(9, 9, 0, 0);
  view_add_border(p_game_view);

  // Map Init
  gp_map = map_create(p_game_view);
  // Load test level
  map_test_init();
  // Draw map
  map_draw(p_game_view, gp_map);

  // Player Init
  s_player =
      (entity_t){.pos = {7, 4}, .ch = '@', .color = COLOR_PAIR(GREEN_BLACK)};
  // Draw it on the screen
  view_draw_entity_at(p_game_view, &s_player);

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
    new_dir = VEC_ZERO;
  }

  // Set player direction
  s_player.dir = new_dir;
}

// Test
bool should_move(vec2_t new_pos) {
  // At border
  if (game_is_colliding_with_border(p_game_view, new_pos))
    return false;
  // Blocking tile
  if (gp_map[new_pos.y][new_pos.x].blocks_movement)
    return false;

  // Get tile at new pos
  tile_t tile = gp_map[new_pos.y][new_pos.x];

  // If it's a movable tile
  if (tile.movable) {
    // Get tile one cell beyond
    vec2_t tile_new_pos = vector_add(new_pos, s_player.dir);
    tile_t next_tile = gp_map[tile_new_pos.y][tile_new_pos.x];

    // Tile is colliding with border / wall or other movable tile
    if (game_is_colliding_with_border(p_game_view, tile_new_pos) ||
        next_tile.blocks_movement || next_tile.movable)
      return false;

    // Update movable tile pos
    gp_map[new_pos.y][new_pos.x] = tile_empty;
    gp_map[tile_new_pos.y][tile_new_pos.x] = tile_rock;

    view_draw_char_at(p_game_view, tile_new_pos.y, tile_new_pos.x,
                      gp_map[tile_new_pos.y][tile_new_pos.x].ch,
                      gp_map[tile_new_pos.y][tile_new_pos.x].color);
  }

  // Move player
  return true;
}

void soko_game_update() {
  vec2_t new_pos = vector_add(s_player.dir, s_player.pos);

  // Check collisions
  if (should_move(new_pos)) {
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

  view_draw(p_game_view);
}

void soko_game_shutdown() {
  // Free map
  map_free();

  delwin(p_game_view->p_view_window);

  free(p_game_view);
}
