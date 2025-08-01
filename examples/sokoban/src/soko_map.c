#include "../soko.h"

int MAP_HEIGHT;
int MAP_WIDTH;

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

tile_t **map_create(view_data_t *p_view) {
  // Set map height
  MAP_HEIGHT = p_view->height;

  // Set map width
  MAP_WIDTH = p_view->width;

  // Allocate for columns (y)
  tile_t **tiles = mem_allocate(MAP_HEIGHT, sizeof(tile_t *));

  // Iterate columns
  for (int y = 0; y < MAP_HEIGHT; y++) {
    // Allocate for rows (x)
    tiles[y] = mem_allocate(MAP_WIDTH, sizeof(tile_t));

    // Init tiles
    for (int x = 0; x < MAP_WIDTH; x++) {
      tiles[y][x].ch = ' ';
      tiles[y][x].blocks_movement = false;
      tiles[y][x].movable = false;
    }
  }

  return tiles;
}

// Test level
void test_level_init() {
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

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      switch (level[y][x]) {
      case 1:
        gp_map[y][x] = tile_wall;
        break;
      case 2:
        gp_map[y][x] = tile_rock;
        break;
      case 0:
        gp_map[y][x] = tile_empty;
        break;
      }
    }
  }
}

void test_map_init() {
  for (int x = 0; x < MAP_WIDTH; x++) {
    if (x > (MAP_WIDTH / 2) - 4 && x < (MAP_WIDTH / 2) + 4)
      gp_map[MAP_HEIGHT / 2][x] = tile_rock;
    else
      gp_map[MAP_HEIGHT / 2][x] = tile_wall;
  }

  gp_map[(MAP_HEIGHT / 2) + 5][MAP_WIDTH / 2] = tile_hole;
}

void map_draw(view_data_t *p_view, tile_t **p_map) {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      view_draw_char_at(p_view, y, x, p_map[y][x].ch, p_map[y][x].color);
    }
  }
}

void map_free() {
  // Free columns
  for (int y = 0; y < MAP_HEIGHT; y++) {
    free(gp_map[y]);
  }

  // Free map
  free(gp_map);
}
