#include "../snake.h"

scene_data_t *gp_snake_title_scene;

// Static global title view
static view_data_t *sp_title_screen;

internal void snake_title_show() {

  int middley = LINES / 2;
  int middlex = (COLS - 46) / 2;

  view_color_begin(sp_title_screen, GREEN_BLACK);
  view_draw_message_at(sp_title_screen, middley - 3, middlex, (char *)".d88888b                    dP                ");
  view_draw_message_at(sp_title_screen, middley - 2, middlex, (char *)"89.    \"'                   88               ");
  view_draw_message_at(sp_title_screen, middley - 1, middlex, (char *)"`Y88888b. 88d888b. .d8888b. 88  .dP  .d8888b. ");
  view_draw_message_at(sp_title_screen, middley    , middlex, (char *)"      `8b 88'  `88 88'  `88 88888\"   88ooood8");
  view_draw_message_at(sp_title_screen, middley + 1, middlex, (char *)"d8'   .8P 88    88 88.  .88 88  `8b. 88.  ... ");
  view_draw_message_at(sp_title_screen, middley + 2, middlex, (char *)" Y88888P  dP    dP `88888P8 dP   `YP `88888P' ");
  view_color_end(sp_title_screen);

  view_draw_message_at(sp_title_screen, middley + 4, (COLS - 26) / 2, "> press any key to start <");

  view_refresh(sp_title_screen);
}

void snake_title_init() {
  // Title View Init
  sp_title_screen = view_create(LINES, COLS, 0, 0);
  gp_snake_title_scene->p_main_view = sp_title_screen;

  // Show Title Screen
  snake_title_show();
}

void snake_title_handle_input() {
  // Get Input - breaking
  if(view_get_input(sp_title_screen) == 'q') {
    g_game.should_close = true;
  } else {
    // Transition to game scene
    game_change_scene(gp_snake_game_scene);
  }
}

// Unused
void snake_title_update() {}

void snake_title_draw() {
  snake_title_show();
}

void snake_title_shutdown() {
  delwin(sp_title_screen->p_view_window);

  free(sp_title_screen);
}
