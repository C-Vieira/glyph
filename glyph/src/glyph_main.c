#include "../glyph.h"

// Entry Point
int main(void) {

  // Get game data from client entry point
  g_game = glyph_main();

  game_init();
  game_run();
  game_shutdown();

  return 0;
}
