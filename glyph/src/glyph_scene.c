#include "../glyph.h"

scene_data_t *scene_create() {
  return (scene_data_t *)mem_allocate(1, sizeof(scene_data_t));
}
