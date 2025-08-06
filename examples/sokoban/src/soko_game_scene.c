#include "../soko.h"

scene_data_t *gp_soko_game_scene;

// Player reference
static entity_t s_player;

// Views
static view_data_t *p_game_view;

// TileMap
tile_map_t g_map_surface;
tile_map_t g_map_ground;

void soko_game_init() {
  // View Init
  p_game_view = view_create(28, 50, 0, 0);
  // p_game_view = view_create(9, 9, 0, 0);
  view_add_border(p_game_view);

  // Map Init
  g_map_surface = map_create(p_game_view);
  g_map_ground = map_create(p_game_view);
  // Load test level
  // test_level_init();
  test_map_init(g_map_ground, g_map_surface);
  // Draw map
  map_draw(p_game_view, g_map_ground);
  map_draw(p_game_view, g_map_surface);

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

  // Get tile and ground tile at new pos
  tile_t tile = map_get_tile_at(g_map_surface, new_pos);
  tile_t ground = map_get_tile_at(g_map_ground, new_pos);

  // Blocking tile
  if (tile.blocks_movement || ground.blocks_movement)
    return false;

  // If it's a movable tile
  if (tile.movable) {
    // Get tile one cell beyond
    vec2_t tile_new_pos = vector_add(new_pos, s_player.dir);
    tile_t next_tile = map_get_tile_at(g_map_surface, tile_new_pos);

    // Tile is colliding with border / wall or other movable tile
    if (view_at_border(p_game_view, tile_new_pos) ||
        next_tile.blocks_movement || next_tile.movable)
      return false;

    // NOTE: We move the tile right here. This happens before the player moves
    // Update movable tile pos
    map_set_tile_at(g_map_surface, tile_empty, new_pos);
    map_set_tile_at(g_map_surface, tile_rock, tile_new_pos);

    view_draw_tile_at(p_game_view, g_map_surface, tile_new_pos);
  }

  // Can move player
  return true;
}

void soko_game_update() {
  vec2_t new_pos = vector_add(s_player.dir, s_player.pos);

  // Check collisions
  if (should_move(new_pos)) {

    // Testing
    // Update special tiles
    for (int i = 0; i < g_hole_positions.occupied; i++) {
      vec2_t hole_pos = ((vec2_t *)g_hole_positions.p_data)[i];

      // If there's a rock tile above
      if (map_get_tile_at(g_map_surface, hole_pos).id == TILE_ROCK &&
          map_get_tile_at(g_map_ground, hole_pos).id == TILE_HOLE) {
        // Plug the hole
        map_set_tile_at(g_map_ground, tile_filled_hole, hole_pos);

        // Delete the rock
        map_set_tile_at(g_map_surface, tile_empty, hole_pos);

        // Update visuals
        view_draw_tile_at(p_game_view, g_map_ground, hole_pos);
      }
    }

    // Clear trail
    vec2_t old_pos = s_player.pos;
    view_draw_tile_at(p_game_view, g_map_ground, old_pos);

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
  // Free array
  array_free(&g_hole_positions);

  // Free map
  map_free(g_map_surface);
  map_free(g_map_ground);

  // Free view windows
  delwin(p_game_view->p_view_window);

  free(p_game_view);
}
