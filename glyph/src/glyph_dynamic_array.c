#include "../glyph.h"

void *array_init(size_t item_size, size_t capacity) {
  void *ptr = 0;
  size_t buffer_size = item_size * capacity + sizeof(dyn_array_header_t);
  // dyn_array_header_t *header = calloc(capacity, buffer_size);
  dyn_array_header_t *header = mem_allocate(capacity, buffer_size);

  if (header) {
    header->capacity = capacity;
    header->occupied = 0;

    ptr = header + 1;

    header->p_data = ptr;
  }

  return ptr;
}

void *array_grow(void *p_array, size_t item_size) {
  dyn_array_header_t *p_header = array_header(p_array);

  // Check for full capacity and dynamically grow the array
  if (p_header->capacity == p_header->occupied) {
    p_header->capacity *= 2;

    p_header = realloc(p_header, p_header->capacity * item_size +
                                     sizeof(dyn_array_header_t));
    if (p_header == NULL) {
      fprintf(stderr, "ERROR: memory reallocation failed\n");
      exit(1);
    }
  }

  // Return pointer to data
  return p_header + 1;
}

void array_free(void *p_array) { free(array_header(p_array)); }
