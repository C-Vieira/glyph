#include "../glyph.h"

dyn_array_t array_create(size_t capacity, data_type_t type) {
  size_t type_size = get_element_size(type);
  dyn_array_t array = {.type = type,
                       .capacity = capacity,
                       .occupied = 0,
                       .p_data = mem_allocate(capacity, type_size)};

  return array;
}

void array_push(dyn_array_t *p_array, size_t index, void *value) {
  // Check for full capacity and dynamically grow the array
  if (p_array->capacity == p_array->occupied) {
    p_array->capacity *= 2;
    p_array->p_data = realloc(
        p_array->p_data, p_array->capacity * get_element_size(p_array->type));
    if (p_array->p_data == NULL) {
      fprintf(stderr, "ERROR: memory reallocation failed\n");
      exit(1);
    }
  }

  // Cast void* value to the appropriate data type
  if (p_array->type == T_INT) {
    int *int_ptr = (int *)value;
    ((int *)p_array->p_data)[index] = *int_ptr;
  } else if (p_array->type == T_ENTITY) {
    entity_t *ent_ptr = (entity_t *)value;
    ((entity_t *)p_array->p_data)[index] = *ent_ptr;
  }
  p_array->occupied++;
}

void array_free(dyn_array_t *p_array) { free(p_array->p_data); }
