#include "../ccrawl.h"

// Temp arena
arena_t *gp_arena;

// Temp room array
room_t *gp_rooms;

internal room_t room_create(int y, int x, int height, int width) {
  int center_middle_y = y + (int)(height / 2);
  int center_middle_x = x + (int)(width / 2);

  return (room_t){.height = height,
                  .width = width,
                  .upper_left_corner_pos = (vec2_t){y, x},
                  .center_pos = (vec2_t){center_middle_y, center_middle_x}};
}

internal void add_room_to_map(bsp_node_t *p_node, tile_map_t map) {
  // Room padding
  int pad_y = randi_range(2, 3);
  int pad_x = randi_range(2, 3);

  // Determine room size
  // Minimum room size 3x5
  int min_height = 3;
  int min_width = 5;
  // int max_height_cap = 9;

  // Caculate available space with padding
  int max_height = p_node->node_height - (pad_y * 2);
  int max_width = p_node->node_width - (pad_x * 2);

  // Randomize room dimensions
  // Max room dimensions should never be less than the minimum
  max_height = MAX(min_height, max_height);
  max_width = MAX(min_width, max_width);

  int room_height = randi_range(min_height, max_height);
  int room_width = randi_range(min_width, max_width);

  // Randomize room position witihin the partition with padding
  int pos_y_range = p_node->node_height - room_height - (pad_y * 2);
  int pos_x_range = p_node->node_width - room_width - (pad_x * 2);

  room_t new_room = room_create(
      p_node->node_pos.y + pad_y + (pos_y_range > 0 ? rand() % pos_y_range : 0),
      p_node->node_pos.x + pad_x + (pos_x_range > 0 ? rand() % pos_x_range : 0),
      room_height, room_width);

  // Add room to rooms array
  array_push(gp_rooms, new_room);

  for (int y = new_room.upper_left_corner_pos.y;
       y < new_room.upper_left_corner_pos.y + new_room.height; y++) {
    for (int x = new_room.upper_left_corner_pos.x;
         x < new_room.upper_left_corner_pos.x + new_room.width; x++) {
      map_set_tile_at(map, tile_empty, (vec2_t){y, x});
    }
  }
}

internal void connect_room_centers(tile_map_t map, vec2_t center_one,
                                   vec2_t center_two) {
  vec2_t temp_pos;
  temp_pos = center_one;

  while (true) {
    if (abs((temp_pos.x - 1) - center_two.x) < abs(temp_pos.x - center_two.x)) {
      temp_pos.x--;
    } else if (abs((temp_pos.x + 1) - center_two.x) <
               abs(temp_pos.x - center_two.x)) {
      temp_pos.x++;
    } else if (abs((temp_pos.y + 1) - center_two.y) <
               abs(temp_pos.y - center_two.y)) {
      temp_pos.y++;
    } else if (abs((temp_pos.y - 1) - center_two.y) <
               abs(temp_pos.y - center_two.y)) {
      temp_pos.y--;
    } else {
      break;
    }

    map_set_tile_at(map, tile_empty, (vec2_t){temp_pos.y, temp_pos.x});
  }
}

// --------------------------------------------
// ----Forward-Declarations--------------------
internal bsp_node_t *split_vertical(bsp_node_t *p_node, int split_pos,
                                    int depth);
internal bsp_node_t *split_horizontal(bsp_node_t *p_node, int split_pos,
                                      int depth);

bsp_node_t *bsp_create(vec2_t pos, int height, int width) {
  bsp_node_t *node = arena_alloc(gp_arena, 1, bsp_node_t);

  node->node_pos = pos;
  node->node_height = height;
  node->node_width = width;
  node->left = NULL;
  node->right = NULL;

  return node;
}

internal bsp_node_t *split(bsp_node_t *p_node, int depth) {
  if (p_node != NULL && depth < NUM_PARTITIONS) {
    // Partiion size is too small for a room, stop splitting
    if (p_node->node_height < MIN_ROOM_SIZE * 2 ||
        p_node->node_width < MIN_ROOM_SIZE * 2) {
      return p_node;
    }
    bool split_vert = rand_bool();
    // Force a split direction if aspect ratio is too high
    if ((float)p_node->node_height / p_node->node_width > 1.25)
      split_vert = false;
    else if ((float)p_node->node_width / p_node->node_height > 1.25)
      split_vert = true;

    int max_split =
        (split_vert ? p_node->node_height : p_node->node_width) - MIN_ROOM_SIZE;

    // There's a division by zero in this file
    // Happens 100% of the time when NUM_PARTITIONS is high enough
    max_split = (max_split > MIN_ROOM_SIZE) ? max_split : MIN_ROOM_SIZE;

    int split_pos = rand() % ((max_split - MIN_ROOM_SIZE) + MIN_ROOM_SIZE);

    if (split_vert) {
      p_node = split_vertical(p_node, split_pos, depth);
    } else {
      p_node = split_horizontal(p_node, split_pos, depth);
    }
  }
  return p_node;
}

internal bsp_node_t *split_vertical(bsp_node_t *p_node, int split_pos,
                                    int depth) {
  p_node->left = bsp_create(p_node->node_pos, p_node->node_height, split_pos);
  p_node->right =
      bsp_create((vec2_t){p_node->node_pos.y, p_node->node_pos.x + split_pos},
                 p_node->node_height, p_node->node_width - split_pos);

  // DEBUG
  mvprintw(0, 0, "On vertical split depth %d", depth);
  // getch();

  p_node->left = split(p_node->left, ++depth);
  p_node->right = split(p_node->right, ++depth);

  return p_node;
}

internal bsp_node_t *split_horizontal(bsp_node_t *p_node, int split_pos,
                                      int depth) {
  p_node->left = bsp_create(p_node->node_pos, split_pos, p_node->node_width);
  p_node->right =
      bsp_create((vec2_t){p_node->node_pos.y + split_pos, p_node->node_pos.x},
                 p_node->node_height - split_pos, p_node->node_width);

  // DEBUG
  mvprintw(0, 0, "On horizontal split depth %d", depth);
  // getch();

  p_node->left = split(p_node->left, ++depth);
  p_node->right = split(p_node->right, ++depth);

  return p_node;
}

internal bool is_leaf(bsp_node_t *p_node) {
  return ((p_node != NULL) && (p_node->left == NULL) &&
          (p_node->right == NULL));
}

internal void traverse(bsp_node_t *p_node, tile_map_t map) {
  if (p_node == NULL)
    return;
  if (is_leaf(p_node)) {

    // DEBUG
    mvprintw(2, 0, "Adding room to map");
    // getch();

    add_room_to_map(p_node, map);
  } else {

    // DEBUG
    mvprintw(1, 0, "On traverse");
    // getch();

    traverse(p_node->left, map);
    traverse(p_node->right, map);
  }
}

internal void bsp_start(tile_map_t map) {
  // Init map (set all tiles to walls)
  for (int y = 0; y < map.MAP_HEIGHT; y++) {
    for (int x = 0; x < map.MAP_WIDTH; x++) {
      map_set_tile_at(map, tile_wall, (vec2_t){y, x});
    }
  }

  bsp_node_t *root =
      bsp_create((vec2_t){0, 0}, map.MAP_HEIGHT - 2, map.MAP_WIDTH - 2);
  root = split(root, 1);

  traverse(root, map);

  // DEBUG
  mvprintw(3, 0, "Connecting rooms");
  // getch();

  // Connect rooms
  for (size_t i = 0; i < array_occupied(gp_rooms); i++) {
    if (i > 0) {
      room_t curr_room = gp_rooms[i];
      room_t prev_room = gp_rooms[i - 1];
      connect_room_centers(map, prev_room.center_pos, curr_room.center_pos);
    }
  }
}

internal void bsp_free() {
  arena_free(gp_arena);
  array_free(gp_rooms);
}

void generator_generate_level(generator_type_e type, tile_map_t *p_map) {
  gp_arena = arena_create(2 * sizeof(bsp_node_t));

  gp_rooms = array_create(room_t, 4);
  bsp_start(*p_map);

  room_t first_room = gp_rooms[0];
  room_t last_room = gp_rooms[array_occupied(gp_rooms) - 1];

  // Place exit
  map_set_tile_at(*p_map, tile_exit, last_room.center_pos);

  p_map->start_pos = first_room.center_pos;
  p_map->exit_pos = last_room.center_pos;

  bsp_free();
}
