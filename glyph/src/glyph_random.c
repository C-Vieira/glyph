#include "../glyph.h"
#include <stdlib.h>

int randi_range(int from, int to) { return (rand() % (to - from) + 1) + from; }
