#include "../soko.h"

scene_data_t *gp_soko_game_scene;

// Player reference
static entity_t s_player;

// Views
static view_data_t *p_game_view;

// Map
tile_map_t g_map;

void soko_game_init() {
  // View Init
  p_game_view = view_create(28, 50, 0, 0);
  // p_game_view = view_create(9, 9, 0, 0);
  view_add_border(p_game_view);

  // Map Init
  g_map = map_create(p_game_view);
  // Load test level
  // test_level_init();
  test_map_init(g_map);
  // Draw map
  map_draw(p_game_view, g_map);

  // Player Init
  s_player =
      (entity_t){.pos = {7, 4}, .ch = '@', .color = COLOR_PAIR(GREEN_BLACK)};
  // Draw it on the screen
  view_draw_entity(p_game_view, &s_player);

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
  if (view_at_border(p_game_view, new_pos))
    return false;

  // Get tile at new pos
  tile_t tile = map_get_tile_at(g_map, new_pos);

  // Blocking tile
  if (tile.blocks_movement)
    return false;

  // If it's a movable tile
  if (tile.movable) {
    // Get tile one cell beyond
    vec2_t tile_new_pos = vector_add(new_pos, s_player.dir);
    tile_t next_tile = map_get_tile_at(g_map, tile_new_pos);

    // TESTING
    // if it's a hole
    if (next_tile.ch == 'X') {
      // Plug the hole
      g_map.p_tiles[tile_new_pos.y][tile_new_pos.x].ch = '.';
      g_map.p_tiles[tile_new_pos.y][tile_new_pos.x].blocks_movement = false;
      // Update visuals
      view_draw_tile_at(p_game_view, g_map, tile_new_pos);

      // Delete the rock
      map_set_tile_at(g_map, tile_empty, new_pos);
      return true;
    }

    // Tile is colliding with border / wall or other movable tile
    if (view_at_border(p_game_view, tile_new_pos) ||
        next_tile.blocks_movement || next_tile.movable)
      return false;

    // Update movable tile pos
    map_set_tile_at(g_map, tile_empty, new_pos);
    map_set_tile_at(g_map, tile_rock, tile_new_pos);

    view_draw_tile_at(p_game_view, g_map, tile_new_pos);
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
    view_draw_tile_at(p_game_view, g_map, old_pos);

    // Update player position
    s_player.pos = new_pos;
  }
}

void soko_game_draw() {
  // Draw player
  view_draw_entity(p_game_view, &s_player);

  view_draw(p_game_view);
}

void soko_game_shutdown() {
  // Free map
  map_free(g_map);

  delwin(p_game_view->p_view_window);

  free(p_game_view);
}
