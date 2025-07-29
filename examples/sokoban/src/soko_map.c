#include "../soko.h"
#include <curses.h>

int MAP_HEIGHT;
int MAP_WIDTH;

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
      tiles[y][x].walkable = true;
    }
  }

  return tiles;
}

void map_test_init() {
  tile_t wall_template = {.ch = '#', .walkable = false};

  for (int x = 0; x < MAP_WIDTH; x++) {
    if (x > (MAP_WIDTH / 2) - 4 && x < (MAP_WIDTH / 2) + 4)
      continue;
    gp_map[MAP_HEIGHT / 2][x] = wall_template;
  }
}

void map_draw(view_data_t *p_view, tile_t **p_map) {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      view_draw_char_at(p_view, y, x, p_map[y][x].ch, COLOR_PAIR(WHITE_BLACK));
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
