#include "../glyph.h"

int randi_range(int from, int to) {
  // Assert test
  assert((to - from) + 1 != 0);

  if ((to - from) + 1 == 0) {
    fprintf(stderr,
            "\nERROR: division by zero in randi_range - from: %d, to: %d\n",
            from, to);
    exit(1);
  }

  return (rand() % ((to - from) + 1)) + from;
}

bool rand_bool() { return rand() % 2; }
