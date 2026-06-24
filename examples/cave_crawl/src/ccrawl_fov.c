#include "../ccrawl.h"

void fov_update(tile_map_t map, entity_t *player) {
  int y, x, distance;
  // Temp constant - Should be part of entity so we can
  // have a varying light radius
  int RADIUS = 15;
  vec2_t target;

  // Set tile at player pos as visible
  tile_t player_tile = map_get_tile_at(map, player->pos);
  player_tile.visible = true;
  player_tile.seen = true;
  map_set_tile_at(map, player_tile, player->pos);

  for (y = player->pos.y - RADIUS; y < player->pos.y + RADIUS; y++) {
    for (x = player->pos.x - RADIUS; x < player->pos.x + RADIUS; x++) {
      target.y = y;
      target.x = x;
      distance = get_distance(player->pos, target);

      if (distance < RADIUS) {
        if (map_is_inside(map, target) &&
            los_is_visible(map, player->pos, target)) {
          tile_t curr_tile = map_get_tile_at(map, target);
          curr_tile.visible = true;
          curr_tile.seen = true;
          map_set_tile_at(map, curr_tile, target);
        }
      }
    }
  }
}

void fov_clear(tile_map_t map, entity_t *player) {
  int y, x;
  int RADIUS = 15;

  for (y = player->pos.y - RADIUS; y < player->pos.y + RADIUS; y++) {
    for (x = player->pos.x - RADIUS; x < player->pos.x + RADIUS; x++) {
      vec2_t curr_pos = (vec2_t){y, x};
      if (map_is_inside(map, curr_pos)) {
        tile_t curr_tile = map_get_tile_at(map, curr_pos);
        curr_tile.visible = false;
        map_set_tile_at(map, curr_tile, curr_pos);
      }
    }
  }
}
