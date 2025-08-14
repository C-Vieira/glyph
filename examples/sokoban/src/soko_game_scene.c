#include "../soko.h"

scene_data_t *gp_soko_game_scene;

// Player reference
static entity_t s_player;

// Views
static view_data_t *sp_game_view;
static view_data_t *sp_victory_screen;
static view_data_t *sp_game_over_screen;

// TileMap
tile_map_t g_map_surface;
tile_map_t g_map_ground;

internal void soko_game_over() {
  // Game Over State
  view_draw(sp_game_over_screen);
  view_draw_message_at(sp_game_over_screen, 2, 1, "   GAME OVER!   ");
  view_draw_message_at(sp_game_over_screen, 3, 1, ">'r' to restart<");
  view_refresh(sp_game_over_screen);
  while (view_get_input(sp_game_view) != 'r')
    ;
  view_clear(sp_game_over_screen);
}

internal void soko_game_won() {
  // Game Over State
  view_draw(sp_game_over_screen);
  view_draw_message_at(sp_game_over_screen, 2, 1, "CONGRATULATIONS!");
  view_draw_message_at(sp_game_over_screen, 3, 1, ">'r' to restart<");
  view_refresh(sp_game_over_screen);
  while (view_get_input(sp_game_view) != 'r')
    ;
  view_clear(sp_game_over_screen);
}

internal void soko_game_start() {
  // Create game view
  sp_game_view = view_create(28, 50, 0, 0);
  // sp_game_view = view_create(9, 9, 0, 0);
  view_add_border(sp_game_view);

  // Map Init
  g_map_surface = map_create(sp_game_view);
  g_map_ground = map_create(sp_game_view);

  // Load test level
  // test_level_init();
  test_map_init(g_map_ground, g_map_surface);

  // Draw map
  map_draw(sp_game_view, g_map_ground);
  map_draw(sp_game_view, g_map_surface);

  // Player Init
  s_player =
      (entity_t){.pos = {7, 4}, .ch = '@', .color = COLOR_PAIR(GREEN_BLACK)};
  // Draw it on the screen
  view_draw_entity(sp_game_view, &s_player);

  view_draw(sp_game_view);
}

void soko_game_init() {
  // Views Init
  sp_victory_screen = view_create(5, 18, 0, 0);
  sp_game_over_screen = view_create(5, 18, 0, 0);

  soko_game_start();
}

void soko_game_handle_input() {
  chtype input = view_get_input(sp_game_view);
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

// Tile pushing and collision logic
// move_dir is the direction to move movable tiles
internal bool should_move(vec2_t new_pos, vec2_t move_dir) {
  // At border
  if (view_at_border(sp_game_view, new_pos))
    return false;

  // Get tile at new pos
  tile_t tile = map_get_tile_at(g_map_surface, new_pos);

  // Blocking tile
  if (tile.blocks_movement)
    return false;

  // If it's a movable tile
  if (tile.movable) {
    // Get tile one cell beyond
    vec2_t tile_new_pos = vector_add(new_pos, move_dir);
    tile_t next_tile = map_get_tile_at(g_map_surface, tile_new_pos);

    // Tile is colliding with border / wall or other movable tile
    if (view_at_border(sp_game_view, tile_new_pos) ||
        next_tile.blocks_movement || next_tile.movable)
      return false;

    // Check if player is in the way
    // Player hasn't moved yet so we add pos + dir
    if (vector_equals(tile_new_pos, vector_add(s_player.pos, s_player.dir)))
      return false;

    // NOTE: We move the tile right here. This happens before the player moves
    // TODO: Move tile should be its own function
    // Update movable tile pos
    // Update W tile positon
    if (tile.id == TILE_LETTER_W)
      g_w_tile_pos = tile_new_pos;

    map_set_tile_at(g_map_surface, tile_empty, new_pos);
    map_set_tile_at(g_map_surface, tile, tile_new_pos);

    view_draw_tile_at(sp_game_view, g_map_surface, tile_new_pos);
  }

  // Can move player
  return true;
}

internal void update_conveyors() {
  // Update conveyor tiles
  for (int i = 0; i < g_conveyor_positions.occupied; i++) {
    vec2_t conv_pos = ((vec2_t *)g_conveyor_positions.p_data)[i];

    // If there's a movable tile above
    if (map_get_tile_at(g_map_surface, conv_pos).movable) {
      // Get conveyor type
      int conv_id = map_get_tile_at(g_map_ground, conv_pos).id;

      vec2_t conv_dir = VEC_ZERO;
      // Set conveyor direction
      switch (conv_id) {
      case TILE_CONVEYOR_RIGHT:
        conv_dir = VEC_RIGHT;
        break;
      case TILE_CONVEYOR_LEFT:
        conv_dir = VEC_LEFT;
        break;
      case TILE_CONVEYOR_DOWN:
        conv_dir = VEC_DOWN;
        break;
      }

      // Update movable tile pos
      tile_t tile = map_get_tile_at(g_map_surface, conv_pos);
      vec2_t next_pos = vector_add(conv_pos, conv_dir);

      // Check for collisions
      if (should_move(next_pos, conv_dir)) {

        // Check if player is in the way
        // Don't move tiles into the player pos
        if (vector_equals(next_pos, s_player.pos))
          break;

        // Move tile
        // Update W tile positon
        if (tile.id == TILE_LETTER_W)
          g_w_tile_pos = next_pos;

        map_set_tile_at(g_map_surface, tile_empty, conv_pos);
        map_set_tile_at(g_map_surface, tile, next_pos);

        // Update visuals
        view_draw_tile_at(sp_game_view, g_map_ground, conv_pos);
        view_draw_tile_at(sp_game_view, g_map_surface, next_pos);

        break;
      }
    }
  }
}

internal void update_player() {
  vec2_t new_pos = vector_add(s_player.dir, s_player.pos);

  // Check collisions
  if (should_move(new_pos, s_player.dir)) {
    // Clear trail
    vec2_t old_pos = s_player.pos;

    // Sanity check for if a tile is ever moved into the player's pos first, so
    // we don't clear it ...
    if (map_get_tile_at(g_map_surface, old_pos).occupied)
      view_draw_tile_at(sp_game_view, g_map_surface, old_pos);
    else
      view_draw_tile_at(sp_game_view, g_map_ground, old_pos);

    // Update player position
    s_player.pos = new_pos;
  }
}

internal void update_holes() {
  // Update hole tiles
  for (int i = 0; i < g_hole_positions.occupied; i++) {
    vec2_t hole_pos = ((vec2_t *)g_hole_positions.p_data)[i];

    // Player is about to move into hole
    if (vector_equals(s_player.pos, hole_pos)) {
      // Game Over
      if (map_get_tile_at(g_map_ground, hole_pos).id == TILE_HOLE) {
        soko_game_over();
        // Restart game
        soko_game_start();
      }
    }

    // If there's a movable tile above
    if (map_get_tile_at(g_map_surface, hole_pos).movable &&
        map_get_tile_at(g_map_ground, hole_pos).id == TILE_HOLE) {
      // Plug the hole
      map_set_tile_at(g_map_ground, tile_filled_hole, hole_pos);

      // If the tile we are about to deleted is the W tile
      // Can't win game, so do game over
      if (map_get_tile_at(g_map_surface, hole_pos).id == TILE_LETTER_W) {
        // Update visuals
        map_set_tile_at(g_map_surface, tile_empty, hole_pos);
        view_draw_tile_at(sp_game_view, g_map_ground, hole_pos);

        // Game Over
        soko_game_over();
        // Restart game
        soko_game_start();
      }

      // Delete the tile
      map_set_tile_at(g_map_surface, tile_empty, hole_pos);

      // Update visuals
      view_draw_tile_at(sp_game_view, g_map_ground, hole_pos);
    }
  }
}

internal void check_win_condition() {
  bool game_won = false;

  vec2_t right = vector_add(g_w_tile_pos, VEC_RIGHT);
  vec2_t down = vector_add(g_w_tile_pos, VEC_DOWN);
  if ((map_get_tile_at(g_map_surface, right).id == TILE_LETTER_I &&
       map_get_tile_at(g_map_surface, vector_add(right, VEC_RIGHT)).id ==
           TILE_LETTER_N) ||
      (map_get_tile_at(g_map_surface, down).id == TILE_LETTER_I &&
       map_get_tile_at(g_map_surface, vector_add(down, VEC_DOWN)).id ==
           TILE_LETTER_N)) {
    game_won = true;
  }

  if (game_won) {
    // Update visuals
    view_draw_entity(sp_game_view, &s_player);

    soko_game_won();
    // Restart game
    soko_game_start();
  }
}

void soko_game_update() {

  // Update moving conveyors
  update_conveyors();
  // Update player
  update_player();
  // Update hole and game over
  update_holes();

  // Check win condition
  check_win_condition();
}

void soko_game_draw() {
  // Draw player
  view_draw_entity(sp_game_view, &s_player);

  view_draw(sp_game_view);
}

void soko_game_shutdown() {
  // Free array
  array_free(&g_hole_positions);

  // Free map
  map_free(g_map_surface);
  map_free(g_map_ground);

  // Free view windows
  delwin(sp_game_view->p_view_window);

  free(sp_game_view);
}
