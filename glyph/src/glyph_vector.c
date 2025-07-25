#include "../glyph.h"

const vec2_t VEC_UP = {-1, 0};
const vec2_t VEC_DOWN = {1, 0};
const vec2_t VEC_LEFT = {0, -1};
const vec2_t VEC_RIGHT = {0, 1};
const vec2_t VEC_ZERO = {0, 0};

vec2_t vector_add(vec2_t vec1, vec2_t vec2) {
  return (vec2_t){.y = vec1.y + vec2.y, .x = vec1.x + vec2.x};
}

bool vector_equals(vec2_t vec1, vec2_t vec2) {
  return (vec1.y == vec2.y) && (vec1.x == vec2.x);
}
