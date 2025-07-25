#include "../glyph.h"

// Initialize and return a queue
queue_t *queue_create(data_type_t type) {
  queue_t *p_queue = mem_allocate(1, sizeof(queue_t));

  p_queue->type = type;
  p_queue->p_front = NULL; // Empty
  p_queue->p_rear = NULL;
  p_queue->length = 0;

  return p_queue;
}

bool queue_is_empty(queue_t *p_queue) { return (p_queue->p_front == NULL); }

// Insert new element at the back
void queue_push(queue_t *p_queue, entity_t *p_entity) {
  node_t *p_new_node = mem_allocate(1, get_element_size(p_queue->type));

  p_new_node->p_data = p_entity;
  p_new_node->p_next = NULL;

  if (p_queue->p_rear == NULL) { // Empty
    p_queue->p_front = p_new_node;
    p_queue->p_rear = p_new_node;
  } else {
    p_queue->p_rear->p_next = p_new_node;
    p_queue->p_rear = p_new_node;
  }
  p_queue->length++;

  // TEST
  // mvprintw(queue->length, 0, "Added segment %zu at y: %d, x: %d",
  // queue->length,
  //         ent->pos.y, ent->pos.x);
  // refresh();
}

// Remove element at the front
void queue_pop(queue_t *p_queue) {
  if (p_queue->p_front == NULL) {
    fprintf(stderr, "ERROR: cannot remove from an empty queue");
    exit(1);
  }

  node_t *p_temp = p_queue->p_front;
  p_queue->p_front = p_queue->p_front->p_next;

  // If queue is now empty
  if (p_queue->p_front == NULL) {
    p_queue->p_rear = NULL;
  }

  // Free node data
  free(p_temp->p_data);
  // Free node
  free(p_temp);
  p_queue->length--;
}

void queue_free(queue_t *p_queue) {
  while (!queue_is_empty(p_queue)) {
    queue_pop(p_queue);
  }
  free(p_queue);
}
