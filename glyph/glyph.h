#ifndef GLYPH_H
#define GLYPH_H

// ----Includes--------------
#include <curses.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ----Defines---------------
// Color pairs
#define WHITE_BLACK 1
#define BLUE_BLACK 2
#define RED_BLACK 3
#define YELLOW_BLACK 4
#define GREEN_BLACK 5
#define BLACK_BLACK 6
#define CYAN_BLACK 7
#define MAGENTA_BLACK 8

// ----Utilities-------------
// Generic Callback
typedef void (*callback_t)();

// ----Vector----------------
typedef struct vec2 {
  int y;
  int x;
} vec2_t;

extern const vec2_t VEC_UP;
extern const vec2_t VEC_DOWN;
extern const vec2_t VEC_LEFT;
extern const vec2_t VEC_RIGHT;
extern const vec2_t VEC_ZERO;

vec2_t vector_add(vec2_t vec1, vec2_t vec2);
bool vector_equals(vec2_t vec1, vec2_t vec2);

// ----Memory----------------
// Type Enum
typedef enum { T_INT, T_ENTITY } data_type_t;

size_t get_element_size(data_type_t type);
void *mem_allocate(size_t num_elements, size_t element_size);

// ----Entity----------------
typedef struct entity {
  vec2_t pos;
  vec2_t dir;
  chtype ch;
  int color;
} entity_t;

typedef struct node {
  entity_t *p_data;
  struct node *p_next;
} node_t; // Entity Node

void entity_move_to(entity_t *p_entity, vec2_t new_pos);

// ----Data-Structures-------
// ----Dynamic-Array---------
typedef struct dyn_array {
  data_type_t type;
  size_t capacity;
  size_t occupied;
  void *p_data;
} dyn_array_t;

dyn_array_t array_create(size_t capacity, data_type_t type);
void array_push(dyn_array_t *p_array, size_t index, void *p_value);
void array_free(dyn_array_t *p_array);

// ----Queue-----------------
typedef struct queue {
  data_type_t type;
  node_t *p_front;
  node_t *p_rear;
  size_t length;
} queue_t;

queue_t *queue_create(data_type_t type);
bool queue_is_empty(queue_t *p_queue);
void queue_push(queue_t *p_queue, entity_t *p_entity);
void queue_pop(queue_t *p_queue);
void queue_free(queue_t *p_queue);

// ----View------------------
typedef struct view_data {
  int height;
  int width;
  int starty;
  int startx;
  WINDOW *p_view_window;
} view_data_t;

view_data_t *view_create(int height, int widht, int yoffset, int xoffset);
void view_set_timeout(view_data_t *p_view, int timeout);
void view_color_begin(view_data_t *p_view, int color);
void view_color_end(view_data_t *p_view);
void view_add_border(view_data_t *p_view);
void view_clear(view_data_t *p_view);
void view_refresh(view_data_t *p_view);
chtype view_get_input(view_data_t *p_view);
void view_get_empty_coords(view_data_t *p_view, int *p_y, int *p_x);
void view_draw_num_at(view_data_t *p_view, int y, int x, int num);
chtype view_get_char_at(view_data_t *p_view, int y, int x);
void view_draw_char_at(view_data_t *p_view, int y, int x, chtype ch, int color);
void view_clear_char_at(view_data_t *p_view, int y, int x);
void view_draw_message_at(view_data_t *p_view, int y, int x, char *p_msg);
void view_draw_entity_at(view_data_t *p_view, entity_t *p_entity);
void view_draw(view_data_t *p_view);

// ----Scene-----------------
typedef struct scene_data {
  view_data_t *p_main_view;
  callback_t init;
  callback_t handle_input;
  callback_t update;
  callback_t draw;
  callback_t shutdown;
} scene_data_t;

scene_data_t *scene_create();

// ----Game------------------
typedef struct game_data {
  bool should_close;
  scene_data_t *curr_scene;
} game_data_t;

extern game_data_t g_game;

bool game_is_colliding_with_border(view_data_t *p_view, vec2_t new_pos);
void game_change_scene(scene_data_t *p_new_scene);
void game_init(void);
void game_run(void);
void game_shutdown(void);

// ----Framework-------------
// Client Entry Point
game_data_t glyph_main();

#endif // !GLYPH_H
