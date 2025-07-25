#include "../snake.h"

entity_t *snake_get_head(snake_player_t *p_snake) {
  return p_snake->p_segments->p_rear->p_data;
}

entity_t *snake_get_tail(snake_player_t *p_snake) {
  return p_snake->p_segments->p_front->p_data;
}

void snake_add_segment(snake_player_t *p_snake, vec2_t pos) {
  entity_t *p_new_segment = mem_allocate(1, sizeof(entity_t));

  p_new_segment->pos = pos;
  p_new_segment->ch = '@';
  p_new_segment->color = COLOR_PAIR(GREEN_BLACK);

  queue_push(p_snake->p_segments, p_new_segment);
}

void snake_remove_segment(snake_player_t *p_snake) {
  queue_pop(p_snake->p_segments);
}

void snake_move_to(snake_player_t *p_snake, vec2_t new_pos) {
  // Add new segment to front of snake (back of the queue)
  snake_add_segment(p_snake, new_pos);

  // Pop tail (front of queue)
  snake_remove_segment(p_snake);
}
