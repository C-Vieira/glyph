#include "../soko.h"

scene_data_t *gp_soko_title_scene;

// Static global title view
static view_data_t *sp_title_screen;

internal void soko_title_show() {
  int middley = LINES / 2;
  // int middlex = (COLS - 46) / 2;

  view_draw_message_at(sp_title_screen, middley, (COLS - 19) / 2,
                       "---   SOKOBAN   ---");
  view_draw_message_at(sp_title_screen, middley + 1, (COLS - 26) / 2,
                       "> press any key to start <");

  view_refresh(sp_title_screen);
}

void soko_title_init() {
  // Title View Init
  sp_title_screen = view_create(LINES, COLS, 0, 0);

  // Show Title Screen
  soko_title_show();
}

void soko_title_handle_input() {
  // Get Input - breaking
  if (view_get_input(sp_title_screen) == 'q') {
    g_game.should_close = true;
  } else {
    // Transition to game scene
    game_change_scene(gp_soko_game_scene);
  }
}

// Unused
void soko_title_update() {}

void soko_title_draw() { soko_title_show(); }

void soko_title_shutdown() {
  delwin(sp_title_screen->p_view_window);

  free(sp_title_screen);
}
