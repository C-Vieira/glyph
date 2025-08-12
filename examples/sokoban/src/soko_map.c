#include "../soko.h"

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

// Test level
void test_level_init(tile_map_t map) {
  /* Test level layout
     "##   ##"  7 x 7 + border = 9 x 9
     "#OO OO#"
     "# OOO #"
     "#O   O#"
     "# OOO #"
     "#     #"
     "#  @  #"  */

  int level[9][9] = {
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
}

dyn_array_t g_hole_positions;
dyn_array_t g_conveyor_positions;

void test_map_init(tile_map_t map_ground, tile_map_t map_surface) {
  // Place walls and rocks on surface
  for (int x = 0; x < map_surface.MAP_WIDTH; x++) {
    if (x > (map_surface.MAP_WIDTH / 2) - 4 &&
        x < (map_surface.MAP_WIDTH / 2) + 4)
      map_surface.p_tiles[map_surface.MAP_HEIGHT / 2][x] = tile_rock;
    else
      map_surface.p_tiles[map_surface.MAP_HEIGHT / 2][x] = tile_wall;
  }

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
  for (int i = 0; i < 8; i++) {
    int y = start_pos.y;
    int x = start_pos.x + i;
    vec2_t pos = {y, x};

    map_ground.p_tiles[y][x] = tile_conveyor_right;
    array_push(&g_conveyor_positions, i, (void *)&pos);
  }
}
