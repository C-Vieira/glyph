#include "../ccrawl.h"

scene_data_t *gp_ccrawl_game_scene;

// Scene memory arena
// static arena_t *gp_arena;

// Player reference
static entity_t s_player;

// Views
static view_data_t *sp_game_view;

// Tilemap
tile_map_t g_map;

void ccrawl_game_init() {
  // Scene memory arena init
  // gp_arena = arena_create(1024);

  // Game view init
  sp_game_view = view_create(35, 90, 0, 0);
  view_add_border(sp_game_view);

  // Tilemap Init
  g_map = map_create(sp_game_view);

  // Call to generator to create level tiles
  generator_generate_level(1, &g_map);

  // Player Init
  s_player = (entity_t){
      .pos = g_map.start_pos, .ch = '@', .color = COLOR_PAIR(GREEN_BLACK)};

  // fov_update(g_map, &s_player);
  map_draw(sp_game_view, g_map);
  view_draw(sp_game_view);

  // Draw player on the screen last
  view_draw_entity(sp_game_view, &s_player);
}

void ccrawl_game_handle_input() {
  chtype input = view_get_input(sp_game_view);
  if (input == 'q') {
    g_game.should_close = true;
  }
  // For Debug
  if (input == 'r') {
    // Shouldn't draw on handle input
    view_clear(sp_game_view);
    generator_generate_level(1, &g_map);
    map_draw(sp_game_view, g_map);
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
  case 'e': {
    // Works well but you shouldn't
    // update and draw on handle input...
    // Player is over the exit
    if (vector_equals(s_player.pos, g_map.exit_pos)) {
      // Regenerate level
      generator_generate_level(RAND_ROOMS, &g_map);
      // Clear visuals
      view_clear(sp_game_view);
      // Reset player pos
      s_player.pos = g_map.start_pos;
      // fov_update(g_map, &s_player);
      map_draw(sp_game_view, g_map);
    }
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
  if (view_at_border(sp_game_view, new_pos)) {
    return;
  }
  // Tile collisions
  if (map_get_tile_at(g_map, new_pos).blocks_movement) {
    return;
  }

  // fov_clear(g_map, &s_player);
  // Update player position
  s_player.pos = new_pos;
  // fov_update(g_map, &s_player);
}

void ccrawl_game_draw() {
  // Draw exit
  tile_t exit_tile = map_get_tile_at(g_map, g_map.exit_pos);
  if (exit_tile.visible) {
    view_draw_char_at(sp_game_view, g_map.exit_pos, exit_tile.ch,
                      exit_tile.color, exit_tile.attribute);
  } else if (exit_tile.seen) {
    view_draw_char_at(sp_game_view, g_map.exit_pos, exit_tile.ch,
                      COLOR_PAIR(WHITE_BLACK), A_DIM);
  }

  map_draw(sp_game_view, g_map);
  view_draw(sp_game_view);

  // Draw player (highest priority)
  view_draw_entity(sp_game_view, &s_player);
}

void ccrawl_game_shutdown() {
  // Free map
  map_free(g_map);

  // arena_free(gp_arena);

  // Free view windows
  delwin(sp_game_view->p_view_window);

  free(sp_game_view);
}
