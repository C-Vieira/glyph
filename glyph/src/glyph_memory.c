#include "../glyph.h"

size_t get_element_size(data_type_t type) {
  size_t type_size = 0;
  if (type == T_INT)
    type_size = sizeof(int);
  else if (type == T_ENTITY)
    type_size = sizeof(entity_t);

  return type_size;
}

void *mem_allocate(size_t num_elements, size_t element_size) {
  void *p_data = calloc(num_elements, element_size);
  if (p_data == NULL) {
    fprintf(stderr, "ERROR: memory allocation failed\n");
    exit(1);
  }

  return p_data;
}
