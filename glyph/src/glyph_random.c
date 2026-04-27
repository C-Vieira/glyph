#include "../glyph.h"
#include <stdlib.h>

int randi_range(int from, int to) {
  if ((to - from) + 1 == 0) {
    fprintf(stderr,
            "\nERROR: division by zero in randi_range - from: %d, to: %d\n",
            from, to);
    exit(1);
  }

  return (rand() % ((to - from) + 1)) + from;
}

bool rand_bool() { return rand() % 2; }
