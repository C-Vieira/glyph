#include "../soko.h"
#include <stdio.h>

// Tile templates
tile_t tile_empty = (tile_t){.id = TILE_EMPTY,
                             .ch = ' ',
                             .occupied = false,
                             .blocks_movement = false,
                             .movable = false};

tile_t tile_wall = (tile_t){.id = TILE_WALL,
                            .ch = '#',
                            .color = COLOR_PAIR(WHITE_BLACK),
                            .occupied = true,
                            .blocks_movement = true,
                            .movable = false};

tile_t tile_rock = (tile_t){.id = TILE_ROCK,
                            .ch = 'O',
                            .color = COLOR_PAIR(YELLOW_BLACK),
                            .occupied = true,
                            .blocks_movement = false,
                            .movable = true};

tile_t tile_hole = (tile_t){.id = TILE_HOLE,
                            .ch = 'X',
                            .color = COLOR_PAIR(RED_BLACK),
                            .occupied = true,
                            .blocks_movement = false,
                            .movable = false};

tile_t tile_filled_hole = (tile_t){.id = TILE_FILLED_HOLE,
                                   .ch = '.',
                                   .color = COLOR_PAIR(RED_BLACK),
                                   .occupied = true,
                                   .blocks_movement = false,
                                   .movable = false};

tile_t tile_conveyor_left = (tile_t){.id = TILE_CONVEYOR_LEFT,
                                     .ch = '<',
                                     .color = COLOR_PAIR(BLUE_BLACK),
                                     .occupied = true,
                                     .blocks_movement = false,
                                     .movable = false};

tile_t tile_conveyor_right = (tile_t){.id = TILE_CONVEYOR_RIGHT,
                                      .ch = '>',
                                      .color = COLOR_PAIR(BLUE_BLACK),
                                      .occupied = true,
                                      .blocks_movement = false,
                                      .movable = false};

tile_t tile_conveyor_down = (tile_t){.id = TILE_CONVEYOR_DOWN,
                                     .ch = 'V',
                                     .color = COLOR_PAIR(BLUE_BLACK),
                                     .occupied = true,
                                     .blocks_movement = false,
                                     .movable = false};

tile_t tile_letter_w = (tile_t){.id = TILE_LETTER_W,
                                .ch = 'W',
                                .color = COLOR_PAIR(WHITE_BLACK),
                                .occupied = true,
                                .blocks_movement = false,
                                .movable = true};

tile_t tile_letter_i = (tile_t){.id = TILE_LETTER_I,
                                .ch = 'I',
                                .color = COLOR_PAIR(WHITE_BLACK),
                                .occupied = true,
                                .blocks_movement = false,
                                .movable = true};

tile_t tile_letter_n = (tile_t){.id = TILE_LETTER_N,
                                .ch = 'N',
                                .color = COLOR_PAIR(WHITE_BLACK),
                                .occupied = true,
                                .blocks_movement = false,
                                .movable = true};

//----Levels--------------------------
// Test level
// internal void test_level_init(tile_map_t map) {
/* Test level layout
   "##   ##"  7 x 7 + border = 9 x 9
   "#OO OO#"
   "# OOO #"
   "#O   O#"
   "# OOO #"
   "#     #"
   "#  @  #"  */

/*int level[9][9] = {
    [0] = {1, 1, 1, 1, 1, 1, 1, 1, 1}, [1] = {1, 1, 0, 0, 0, 0, 0, 1, 1},
    [2] = {1, 1, 1, 0, 0, 0, 1, 1, 1}, [3] = {1, 1, 2, 2, 0, 2, 2, 1, 1},
    [4] = {1, 1, 0, 2, 2, 2, 0, 1, 1}, [5] = {1, 1, 2, 0, 0, 0, 2, 1, 1},
    [6] = {1, 1, 0, 2, 2, 2, 0, 1, 1}, [7] = {1, 1, 0, 0, 0, 0, 0, 1, 1},
    [8] = {1, 1, 1, 1, 1, 1, 1, 1, 1}};

for (int y = 0; y < map.MAP_HEIGHT; y++) {
  for (int x = 0; x < map.MAP_WIDTH; x++) {
    switch (level[y][x]) {
    case 1:
      map.p_tiles[y][x] = tile_wall;
      break;
    case 2:
      map.p_tiles[y][x] = tile_rock;
      break;
    case 0:
      map.p_tiles[y][x] = tile_empty;
      break;
    }
  }
}
}*/

dyn_array_t g_hole_positions;
dyn_array_t g_conveyor_positions;
vec2_t g_w_tile_pos;

// TODO: Level layout loader
internal void load_level_layout(int level_array[][50], int rows) {
  FILE *p_file;

  p_file = fopen("examples/sokoban/levels/level_test_layout_surface.txt", "r");

  if (p_file == NULL) {
    fprintf(stderr, "ERROR: couldn't open specified file\n");
    exit(1);
  }

  int tile_id = 0;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < 50; x++) {
      if (fscanf(p_file, "%d", &tile_id) == EOF)
        break;
      level_array[y][x] = tile_id;
    }
  }

  fclose(p_file);
}

internal vec2_t level_test_init(tile_map_t map_ground, tile_map_t map_surface) {
  // Place walls and rocks on surface
  for (int x = 0; x < map_surface.MAP_WIDTH; x++) {
    if (x > (map_surface.MAP_WIDTH / 2) - 4 &&
        x < (map_surface.MAP_WIDTH / 2) + 4)
      map_surface.p_tiles[map_surface.MAP_HEIGHT / 2][x] = tile_rock;
    else
      map_surface.p_tiles[map_surface.MAP_HEIGHT / 2][x] = tile_wall;
  }

  vec2_t player_start_pos = {7, 4};

  g_w_tile_pos = (vec2_t){5, map_surface.MAP_HEIGHT / 2};
  map_set_tile_at(map_surface, tile_letter_w, g_w_tile_pos);
  map_set_tile_at(map_surface, tile_letter_i,
                  vector_add(g_w_tile_pos, VEC_LEFT));
  map_set_tile_at(map_surface, tile_letter_n,
                  vector_add(g_w_tile_pos, VEC_RIGHT));

  g_hole_positions = array_create(4, T_VEC);
  g_conveyor_positions = array_create(4, T_VEC);

  vec2_t start_pos = {.y = (map_ground.MAP_HEIGHT / 2) + 5,
                      .x = (map_ground.MAP_WIDTH / 2) - 1};

  // Place holes on ground
  for (int i = 0; i < 1; i++) {
    int y = start_pos.y;
    int x = start_pos.x + 8; // start_pos.x + i;
    vec2_t pos = {y, x};

    map_ground.p_tiles[y][x] = tile_hole;
    array_push(&g_hole_positions, i, (void *)&pos);
  }

  // Place conveyors on ground
  int y;
  int x;
  // Right
  for (int i = 0; i < 8; i++) {
    y = start_pos.y;
    x = start_pos.x + i;
    vec2_t pos = {y, x};

    map_ground.p_tiles[y][x] = tile_conveyor_right;
    array_push(&g_conveyor_positions, i, (void *)&pos);
  }
  // Down
  for (int i = 0; i < 4; i++) {
    y = start_pos.y + i;
    vec2_t pos = {y, x};

    map_ground.p_tiles[y][x] = tile_conveyor_down;
    array_push(&g_conveyor_positions, g_conveyor_positions.occupied,
               (void *)&pos);
  }
  // Left
  for (int i = 0; i < 8; i++) {
    x = start_pos.x + i;
    vec2_t pos = {y, x};

    map_ground.p_tiles[y][x] = tile_conveyor_left;
    array_push(&g_conveyor_positions, g_conveyor_positions.occupied,
               (void *)&pos);
  }

  return player_start_pos;
}

internal vec2_t level_easy_init(tile_map_t map_ground, tile_map_t map_surface) {
  vec2_t player_start_pos = {1, 1};

  g_hole_positions = array_create(4, T_VEC);
  g_conveyor_positions = array_create(4, T_VEC);

  // Load level layout from file
  int level_surface[map_surface.MAP_HEIGHT][map_surface.MAP_WIDTH];
  load_level_layout(level_surface, map_surface.MAP_HEIGHT);

  // Place surface tiles
  for (int y = 0; y < map_surface.MAP_HEIGHT; y++) {
    for (int x = 0; x < map_surface.MAP_WIDTH; x++) {
      switch (level_surface[y][x]) {
      case TILE_EMPTY:
        map_surface.p_tiles[y][x] = tile_empty;
        break;
      case TILE_WALL:
        map_surface.p_tiles[y][x] = tile_wall;
        break;
      case TILE_ROCK:
        map_surface.p_tiles[y][x] = tile_rock;
        break;
      }
    }
  }

  return player_start_pos;
}

internal vec2_t level_medium_init(tile_map_t map_ground,
                                  tile_map_t map_surface) {
  vec2_t player_start_pos = {1, 1};

  g_hole_positions = array_create(4, T_VEC);
  g_conveyor_positions = array_create(4, T_VEC);

  // TODO: Level layout

  return player_start_pos;
}

internal vec2_t level_hard_init(tile_map_t map_ground, tile_map_t map_surface) {
  vec2_t player_start_pos = {1, 1};

  g_hole_positions = array_create(4, T_VEC);
  g_conveyor_positions = array_create(4, T_VEC);

  // TODO: Level layout

  return player_start_pos;
}

vec2_t soko_level_init(int level_id, tile_map_t map_ground,
                       tile_map_t map_surface) {

  switch (level_id) {
  case LEVEL_TEST:
    return level_test_init(map_ground, map_surface);
  case LEVEL_EASY:
    return level_easy_init(map_ground, map_surface);
  case LEVEL_MEDIUM:
    return level_medium_init(map_ground, map_surface);
  case LEVEL_HARD:
    return level_hard_init(map_ground, map_surface);
  }

  return (vec2_t){1, 1};
}
