#include "../glyph.h"

tile_map_t map_create(view_data_t *p_view) {
  tile_map_t map = {// Set map height
                    .MAP_HEIGHT = p_view->height,

                    // Set map width
                    .MAP_WIDTH = p_view->width};

  // Allocate for columns (y)
  map.p_tiles = mem_allocate(map.MAP_HEIGHT, sizeof(tile_t *));

  // Iterate columns
  for (int y = 0; y < map.MAP_HEIGHT; y++) {
    // Allocate for rows (x)
    map.p_tiles[y] = mem_allocate(map.MAP_WIDTH, sizeof(tile_t));

    // Init tiles
    for (int x = 0; x < map.MAP_WIDTH; x++) {
      map.p_tiles[y][x].ch = ' ';
      map.p_tiles[y][x].occupied = false;
      map.p_tiles[y][x].blocks_movement = false;
      map.p_tiles[y][x].movable = false;
    }
  }

  return map;
}

tile_t map_get_tile_at(tile_map_t map, vec2_t pos) {
  return map.p_tiles[pos.y][pos.x];
}

void map_set_tile_at(tile_map_t map, tile_t tile, vec2_t pos) {
  map.p_tiles[pos.y][pos.x] = tile;
}

// Could maybe be a view function instead
void map_draw(view_data_t *p_view, tile_map_t map) {
  for (int y = 0; y < map.MAP_HEIGHT; y++) {
    for (int x = 0; x < map.MAP_WIDTH; x++) {
      // Only draw non empty tiles
      if (map.p_tiles[y][x].occupied)
        view_draw_char_at(p_view, (vec2_t){y, x}, map.p_tiles[y][x].ch,
                          map.p_tiles[y][x].color);
    }
  }
}

void map_free(tile_map_t map) {
  // Free columns
  for (int y = 0; y < map.MAP_HEIGHT; y++) {
    free(map.p_tiles[y]);
  }

  // Free map
  free(map.p_tiles);
}
