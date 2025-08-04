#include "../glyph.h"

view_data_t *view_create(int height, int width, int yoffset, int xoffset) {
  int starty = ((LINES - height) + yoffset) / 2;
  int startx = ((COLS - width) + xoffset) / 2;

  view_data_t *p_new_view = (view_data_t *)mem_allocate(1, sizeof(view_data_t));

  p_new_view->height = height;
  p_new_view->width = width;
  p_new_view->starty = starty;
  p_new_view->startx = startx;
  p_new_view->p_view_window = newwin(height, width, starty, startx);

  // This was the problem...
  // DO NOT initialize a pointer to struct with a compound literal
  // This goes out of scope when this function finishes
  // Leading to undefined behaviour (?)
  /*new_view =
      &(view_data_t){.m_height = height,
                     .m_width = width,
                     .m_starty = starty,
                     .m_startx = startx,
                     .p_view_window = newwin(height, width, starty, startx)};*/

  return p_new_view;
}

void view_set_timeout(view_data_t *p_view, int timeout) {
  wtimeout(p_view->p_view_window, timeout);
}

void view_color_begin(view_data_t *p_view, int color) {
  wattron(p_view->p_view_window, COLOR_PAIR(color));
}

void view_color_end(view_data_t *p_view) {
  wattroff(p_view->p_view_window, COLOR_PAIR(WHITE_BLACK));
}

void view_add_border(view_data_t *p_view) { box(p_view->p_view_window, 0, 0); }

bool view_at_border(view_data_t *p_view, vec2_t new_pos) {
  if (new_pos.y <= 0 || new_pos.y >= p_view->height - 1 || new_pos.x <= 0 ||
      new_pos.x == p_view->width - 1)
    return true;
  return false;
}

void view_clear(view_data_t *p_view) {
  wclear(p_view->p_view_window);
  wrefresh(p_view->p_view_window);
}

void view_refresh(view_data_t *p_view) { wrefresh(p_view->p_view_window); }

chtype view_get_input(view_data_t *p_view) {
  return wgetch(p_view->p_view_window);
}

void view_get_empty_coords(view_data_t *p_view, int *p_y, int *p_x) {
  // Positions should be > 0
  // and < view_height/widht - 1
  // (-2 to account for +1 outside)
  while (mvwinch(p_view->p_view_window,
                 *p_y = (rand() % (p_view->height - 2)) + 1,
                 *p_x = (rand() % (p_view->width - 2)) + 1) != ' ')
    ;

  // TEST
  // mvprintw(0, 1, "Fruit pos is: y %d x %d", g_fruit.pos.y, g_fruit.pos.x);
  // mvprintw(1, 1, "View height is: %d", view->m_height);
  // mvprintw(2, 1, "View width  is: %d", view->m_width);
  // refresh();
}

void view_draw_num_at(view_data_t *p_view, int y, int x, int num) {
  mvwprintw(p_view->p_view_window, y, x, "%d", num);
}

chtype view_get_char_at(view_data_t *p_view, vec2_t pos) {
  return mvwinch(p_view->p_view_window, pos.y, pos.x);
}

void view_draw_char_at(view_data_t *p_view, vec2_t pos, chtype ch, int color) {
  mvwaddch(p_view->p_view_window, pos.y, pos.x, ch | color);
}

void view_clear_char_at(view_data_t *p_view, vec2_t pos) {
  mvwaddch(p_view->p_view_window, pos.y, pos.x, ' ');
}

void view_draw_message_at(view_data_t *p_view, int y, int x, char *p_msg) {
  mvwprintw(p_view->p_view_window, y, x, "%s", p_msg);
}

void view_draw_entity_at(view_data_t *p_view, entity_t *p_entity) {
  view_draw_char_at(p_view, p_entity->pos, p_entity->ch, p_entity->color);
}

void view_draw(view_data_t *p_view) {
  view_add_border(p_view);
  view_refresh(p_view);
}
