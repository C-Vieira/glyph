#include "../glyph.h"

// Global game reference
game_data_t g_game;

static void game_curses_setup() {
  initscr();
  noecho();
  curs_set(0);

  if (has_colors()) {
    start_color();

    init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);     // blue
    init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);       // white
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);         // red
    init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);   // yellow
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);     // green
    init_pair(BLACK_BLACK, COLOR_BLACK, COLOR_BLACK);     // black
    init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);       // cyan
    init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK); // magenta

  } else {
    mvprintw(20, 50, "Your system doesn't support color. Can't start game!");
    getch();
  }
}

bool game_is_colliding_with_border(view_data_t *p_view, vec2_t new_pos) {
  if (new_pos.y <= 0 || new_pos.y >= p_view->height - 1 || new_pos.x <= 0 ||
      new_pos.x == p_view->width - 1)
    return true;
  return false;
}

void game_change_scene(scene_data_t *p_new_scene) {
  // Cleanup
  g_game.p_curr_scene->c_shutdown();

  // Set new scene and init
  g_game.p_curr_scene = p_new_scene;
  p_new_scene->c_init();
}

void game_init() {
  // Rand Init
  srand(time(NULL));

  // Curses Init
  game_curses_setup();

  g_game.p_curr_scene->c_init();
}

void game_run() {
  while (!g_game.should_close) {
    g_game.p_curr_scene->c_handle_input();
    g_game.p_curr_scene->c_update();
    g_game.p_curr_scene->c_draw();
  }
}

void game_shutdown() {
  g_game.p_curr_scene->c_shutdown();
  endwin();
}
