#include "../ccrawl.h"
#include <math.h>

bool los_is_visible(tile_map_t map, vec2_t origin, vec2_t target) {
  int t, x, y, abs_delta_x, abs_delta_y, sign_x, sign_y, delta_x, delta_y;

  delta_x = origin.x - target.x;
  delta_y = origin.y - target.y;

  abs_delta_x = abs(delta_x);
  abs_delta_y = abs(delta_y);

  sign_x = SIGN(delta_x);
  sign_y = SIGN(delta_y);

  x = target.x;
  y = target.y;

  if (abs_delta_x > abs_delta_y) {
    t = abs_delta_y * 2 - abs_delta_x;

    do {
      if (t >= 0) {
        y += sign_y;
        t -= abs_delta_x * 2;
      }

      x += sign_x;
      t += abs_delta_y * 2;

      if (x == origin.x && y == origin.y) {
        return true;
      }
    } while (map_get_tile_at(map, (vec2_t){y, x}).transparent);

    return false;
  } else {
    t = abs_delta_x * 2 - abs_delta_y;

    do {
      if (t >= 0) {
        x += sign_x;
        t -= abs_delta_y * 2;
      }

      y += sign_y;
      t += abs_delta_x * 2;

      if (x == origin.x && y == origin.y) {
        return true;
      }
    } while (map_get_tile_at(map, (vec2_t){y, x}).transparent);

    return false;
  }
}

int get_distance(vec2_t origin, vec2_t target) {
  double dy, dx;
  int distance;

  dy = target.y - origin.y;
  dx = target.x - origin.x;
  distance = floor(sqrt((dx * dx) + (dy * dy)));

  return distance;
}
