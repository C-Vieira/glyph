#include "../glyph.h"

void entity_move_to(entity_t *p_entity, vec2_t new_pos) {
  p_entity->pos = new_pos;
}
