#include "../glyph.h"

arena_t *arena_create(size_t capacity) {
  // Allocate arena
  arena_t *p_arena = mem_allocate(1, sizeof(arena_t));

  // Allocate buffer data
  p_arena->p_buffer = mem_allocate(capacity, sizeof(char));

  p_arena->offset = 0;
  p_arena->capacity = capacity;

  return p_arena;
}

void *arena_push(arena_t *p_arena, size_t size) {
  // Aligned offset
  int64_t aligned_offset = ALIGN_UP_POW2(p_arena->offset, ARENA_ALIGNMENT);

  // NOTE: This resize logic causes problems with pointers in the arena
  // realloc may move the entire block to a different space in RAM and
  // free the old data, leaving pointers that previously pointed to data
  // inside the arena "dangling" with outdated addresses

  // TODO: Extend arena implementation with a chained memory regions / blocks
  // approach.
  if (aligned_offset + size > p_arena->capacity) {
    // Realocate arena
    p_arena->capacity *= 2;
    p_arena->p_buffer = realloc(p_arena->p_buffer, p_arena->capacity);

    if (p_arena->p_buffer == NULL) {
      fprintf(stderr, "ERROR: arena reallocation failed\n");
      exit(1);
    }
  }

  // Get a pointer to the end of arena's memory region
  // void *ptr = p_arena->p_buffer + p_arena->offset;
  void *ptr = p_arena->p_buffer + aligned_offset;

  // Update offset
  // p_arena->offset += size;
  p_arena->offset = aligned_offset + size;

  // TEST
  // mvprintw(0, 0, "\nAllocated something with size %zu\n", size);

  return ptr;
}

void arena_free(arena_t *p_arena) {
  // TEST
  // printf("\nFreeing the arena\n");

  if (p_arena) {
    free(p_arena->p_buffer);
    free(p_arena);
  }
}
