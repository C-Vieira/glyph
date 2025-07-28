#include "../soko.h"

scene_data_t *gp_soko_game_scene;

// Views
static view_data_t *p_game_view;

void soko_game_init() {
  p_game_view = view_create(28, 50, 0, 0);
  view_add_border(p_game_view);

  view_draw(p_game_view);
}

void soko_game_handle_input() {
  chtype input = view_get_input(p_game_view);
  if (input == 'q')
    g_game.should_close = true;
}

void soko_game_update() {}

void soko_game_draw() { view_draw(p_game_view); }

void soko_game_shutdown() {
  delwin(p_game_view->p_view_window);

  free(p_game_view);
}
