#include "../ccrawl.h"
#include <curses.h>
#include <stdlib.h>

// Tile templates
tile_t tile_empty = (tile_t){
    .id = 0,
    .ch = ':',
    .color = COLOR_PAIR(WHITE_BLACK),
    .attribute = A_NORMAL,
    .blocks_movement = false,
    .movable = false,
    .occupied = true,
    .transparent = true,
    .visible = false,
    .seen = false,
};

tile_t tile_wall = (tile_t){
    .id = 1,
    .ch = '#',
    .color = COLOR_PAIR(WHITE_BLACK),
    .attribute = A_REVERSE,
    .blocks_movement = true,
    .movable = false,
    .occupied = true,
    .transparent = false,
    .visible = false,
    .seen = false,
};

tile_t tile_exit = (tile_t){
    .id = 2,
    .ch = '<',
    .color = COLOR_PAIR(YELLOW_BLACK),
    .attribute = A_NORMAL,
    .blocks_movement = false,
    .movable = false,
    .transparent = true,
    .visible = false,
    .seen = false,
};

// ----Generator--------------
// ----Cellular-Automata-Caves
// Cell data
typedef struct {
  int state;
  bool reachable;
} cell_t;

// Globals
cell_t **g_world;
cell_t **g_buffer;

int WORLD_HEIGHT;
int WORLD_WIDTH;

int num_generations = 10;

internal cell_t **init_world_cells() {
  // Allocate columns(y)
  cell_t **cells = mem_allocate(WORLD_HEIGHT, sizeof(cell_t *));

  for (int y = 0; y < WORLD_HEIGHT; y++) {
    // Allocate rows(x)
    cells[y] = mem_allocate(WORLD_WIDTH, sizeof(cell_t));

    for (int x = 0; x < WORLD_WIDTH; x++) {
      cells[y][x].state =
          rand() % 11 > 4 ? 1 : 0; // Greater chance for an alive cell
                                   // rand() % 10 >= 5 ? 1 : 0;
    }
  }

  return cells;
}

internal int count_neighbors(int x, int y) {
  int sum = 0;
  int rows, cols;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      cols = (x + i + WORLD_HEIGHT) %
             WORLD_HEIGHT; // Relative position + wraparound
      rows = (y + j + WORLD_WIDTH) % WORLD_WIDTH;
      sum += g_world[cols][rows].state;
    }
  }
  return g_world[x][y].state == 1 ? --sum : sum;
}

internal void update_world(int type_flag) {
  for (int i = 0; i < WORLD_HEIGHT; i++) {
    for (int j = 0; j < WORLD_WIDTH; j++) {

      int n_sum = count_neighbors(i, j); // Check neighborhood

      // Compute New States
      if (type_flag == 1) {
        // CAVES
        if (n_sum >= 6 && g_world[i][j].state == 0)
          g_buffer[i][j].state = 1;
        else if (n_sum <= 3 && g_world[i][j].state == 1)
          g_buffer[i][j].state = 0;
      } else {
        // MAZE
        if (n_sum == 3 && g_world[i][j].state == 0)
          g_buffer[i][j].state = 1;
        else if (n_sum > 0 && n_sum < 5 && g_world[i][j].state == 1)
          g_buffer[i][j].state = 1;
        else
          g_buffer[i][j].state = 0;
      }
    }
  }
  // Update world with new values
  for (int i = 0; i < WORLD_HEIGHT; i++)
    for (int j = 0; j < WORLD_WIDTH; j++)
      g_world[i][j].state = g_buffer[i][j].state;
}

internal void free_world(void) {
  for (int y = 0; y < WORLD_HEIGHT; y++) {
    free(g_world[y]);
    free(g_buffer[y]);
  }
  free(g_world);
  free(g_buffer);
}

// ----Random-Rooms-----------
internal room_t create_room(int y, int x, int height, int width) {
  int center_middle_y = y + (int)(height / 2);
  int center_middle_x = x + (int)(width / 2);

  return (room_t){.height = height,
                  .width = width,
                  .upper_left_corner_pos = (vec2_t){y, x},
                  .center_pos = (vec2_t){center_middle_y, center_middle_x}};
}

internal void add_room_to_map(room_t room, tile_map_t map) {
  for (int y = room.upper_left_corner_pos.y;
       y < room.upper_left_corner_pos.y + room.height; y++) {
    for (int x = room.upper_left_corner_pos.x;
         x < room.upper_left_corner_pos.x + room.width; x++) {
      map_set_tile_at(map, tile_empty, (vec2_t){y, x});
    }
  }
}

internal void connect_room_centers(tile_map_t map, vec2_t center_one,
                                   vec2_t center_two) {
  vec2_t temp_pos;
  temp_pos.y = center_one.y;
  temp_pos.x = center_one.x;

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

internal vec2_t init_map_rooms(tile_map_t map) {
  // Room gen values
  int y, x, height, width, n_rooms;
  n_rooms = randi_range(5, 15);

  // Temp room array
  room_t *rooms = mem_allocate(n_rooms, sizeof(room_t));

  vec2_t start_pos;

  // Init map
  for (int y = 0; y < map.MAP_HEIGHT; y++) {
    for (int x = 0; x < map.MAP_WIDTH; x++) {
      map_set_tile_at(map, tile_wall, (vec2_t){y, x});
    }
  }

  // Generate and place rooms
  for (int i = 0; i < n_rooms; i++) {
    y = randi_range(1, WORLD_HEIGHT - 10);
    x = randi_range(1, WORLD_WIDTH - 20);
    height = randi_range(3, 9);
    width = randi_range(5, 19);

    rooms[i] = create_room(y, x, height, width);
    add_room_to_map(rooms[i], map);

    if (i > 0) {
      connect_room_centers(map, rooms[i - 1].center_pos, rooms[i].center_pos);
    }
  }

  start_pos.y = rooms[0].center_pos.y;
  start_pos.x = rooms[0].center_pos.x;

  free(rooms);

  return start_pos;
}

// TEST
void flood_fill(int x, int y) {
  if ((g_world[y][x].state == 0) && (!g_world[y][x].reachable)) {
    g_world[y][x].reachable = true;
  } else {
    return;
  }
  flood_fill(y, x + 1);
  flood_fill(y, x - 1);
  flood_fill(y + 1, x);
  flood_fill(y - 1, x);
}

bool are_all_tiles_reachable(void) {
  for (int count1 = 0; count1 < WORLD_HEIGHT; count1++)
    for (int count2 = 0; count2 < WORLD_WIDTH; count2++)
      if ((g_world[count1][count2].state == 0) &&
          (!g_world[count1][count2].reachable))
        return false;
  return true;
}

// ----API--------------------
/*
vec2_t generator_generate_level(generator_type_e type, tile_map_t *p_map) {
  // Set globals
  WORLD_HEIGHT = p_map->MAP_HEIGHT;
  WORLD_WIDTH = p_map->MAP_WIDTH;

  switch (type) {
  case RAND_ROOMS: {
    return init_map_rooms(*p_map);
    break;
  }
  case CA_CAVES: {
    g_world = init_world_cells();
    g_buffer = init_world_cells();

    // Generate level
    // Initial maze test
    // for (int j = 0; j < 25; j++) {
    //   update_world(0);
    // }
    // Turn maze into cave
    for (int i = 0; i < num_generations; i++) {
      update_world(1);
    }
    //  Parse world to map
    for (int y = 0; y < WORLD_HEIGHT; y++) {
      for (int x = 0; x < WORLD_WIDTH; x++) {
        g_world[y][x].state == 1
            ? map_set_tile_at(*p_map, tile_wall, (vec2_t){y, x})
            : map_set_tile_at(*p_map, tile_empty, (vec2_t){y, x});
      }
    }

    free_world();

    // Temp
    return (vec2_t){5, 5};
    break;
  }
  default:
    return (vec2_t){5, 5};
  }
}
*/
