#include "../soko.h"

// Tile templates
tile_t tile_empty =
    (tile_t){.ch = ' ', .blocks_movement = false, .movable = false};

tile_t tile_wall = (tile_t){.ch = '#',
                            .color = COLOR_PAIR(WHITE_BLACK),
                            .blocks_movement = true,
                            .movable = false};

tile_t tile_rock = (tile_t){.ch = 'O',
                            .color = COLOR_PAIR(YELLOW_BLACK),
                            .blocks_movement = false,
                            .movable = true};

tile_t tile_hole = (tile_t){.ch = 'X',
                            .color = COLOR_PAIR(RED_BLACK),
                            .blocks_movement = true,
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

void test_map_init(tile_map_t map) {
  for (int x = 0; x < map.MAP_WIDTH; x++) {
    if (x > (map.MAP_WIDTH / 2) - 4 && x < (map.MAP_WIDTH / 2) + 4)
      map.p_tiles[map.MAP_HEIGHT / 2][x] = tile_rock;
    else
      map.p_tiles[map.MAP_HEIGHT / 2][x] = tile_wall;
  }

  map.p_tiles[(map.MAP_HEIGHT / 2) + 5][map.MAP_WIDTH / 2] = tile_hole;
}
