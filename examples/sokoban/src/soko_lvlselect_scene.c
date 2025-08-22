#include "../soko.h"
#include <curses.h>
#include <menu.h>
#include <string.h>

internal void set_curr_level(int lvl_id) { curr_level = lvl_id; }

// Test array size macro
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

// Menu stuff
ITEM **menu_choices;
MENU *lvl_menu;
int n_choices;
ITEM *curr_item;

scene_data_t *gp_soko_lvlselect_scene;

// Static global menu view
static view_data_t *sp_menu_screen;

void soko_lvlselect_init() {
  char *choices[] = {"Level 1", "Level 2", "Level 3"};

  // Menu View Init
  sp_menu_screen = view_create(LINES, COLS, 0, 0);

  n_choices = ARRAY_SIZE(choices);
  menu_choices = (ITEM **)mem_allocate(n_choices + 1, sizeof(ITEM *));

  // Create menu items
  for (int i = 0; i < n_choices; i++) {
    menu_choices[i] = new_item(choices[i], NULL);
    // Set user pointer
    set_item_userptr(menu_choices[i], set_curr_level);
  }

  // Set last item as null
  menu_choices[n_choices] = (ITEM *)NULL;

  // Create the menu
  lvl_menu = new_menu((ITEM **)menu_choices);

  // Set menu to view window
  set_menu_win(lvl_menu, sp_menu_screen->p_view_window);
  set_menu_sub(lvl_menu, derwin(sp_menu_screen->p_view_window, 6, 38, LINES / 2,
                                COLS / 2));

  // Set menu mark to '>'
  set_menu_mark(lvl_menu, ">");

  view_draw_message_at(sp_menu_screen, (LINES - 4) / 2, COLS / 2,
                       "Level Select");

  // Draw it on the screen
  post_menu(lvl_menu);

  view_refresh(sp_menu_screen);
}

void soko_lvlselect_handle_input() {
  // Get Input - breaking
  chtype input = view_get_input(sp_menu_screen);
  if (input == 'q') {
    // Transition to title scene
    game_change_scene(gp_soko_title_scene);
  } else {
    ITEM *curr;
    // Menu item callback
    void (*p)(int);
    switch (input) {
    case 'j':
      menu_driver(lvl_menu, REQ_DOWN_ITEM);
      break;
    case 'k':
      menu_driver(lvl_menu, REQ_UP_ITEM);
      break;
    case 10: // Enter
      curr = current_item(lvl_menu);
      p = item_userptr(curr);
      p(item_index(curr));
      pos_menu_cursor(lvl_menu);

      // Transition to game scene
      game_change_scene(gp_soko_game_scene);
      break;
    }
  }
}

void soko_lvlselect_update() {}

void soko_lvlselect_draw() {}

void soko_lvlselect_shutdown() {
  delwin(sp_menu_screen->p_view_window);

  for (int i = 0; i < n_choices; i++) {
    free_item(menu_choices[i]);
  }
  free_menu(lvl_menu);

  free(sp_menu_screen);
}
