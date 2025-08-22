#include "../soko.h"
#include <stdbool.h>
#include <string.h>

// Tile templates
tile_t tile_empty = (tile_t){.id = TILE_EMPTY,
                             .ch = ' ',
                             .occupied = false,
                             .blocks_movement = false,
                             .movable = false};

tile_t tile_wall = (tile_t){.id = TILE_WALL,
                            .ch = '#',
                            .color = COLOR_PAIR(WHITE_BLACK),
                            .occupied = true,
                            .blocks_movement = true,
                            .movable = false};

tile_t tile_rock = (tile_t){.id = TILE_ROCK,
                            .ch = 'O',
                            .color = COLOR_PAIR(YELLOW_BLACK),
                            .occupied = true,
                            .blocks_movement = false,
                            .movable = true};

tile_t tile_hole = (tile_t){.id = TILE_HOLE,
                            .ch = 'X',
                            .color = COLOR_PAIR(RED_BLACK),
                            .occupied = true,
                            .blocks_movement = false,
                            .movable = false};

tile_t tile_filled_hole = (tile_t){.id = TILE_FILLED_HOLE,
                                   .ch = '.',
                                   .color = COLOR_PAIR(RED_BLACK),
                                   .occupied = true,
                                   .blocks_movement = false,
                                   .movable = false};

tile_t tile_conveyor_left = (tile_t){.id = TILE_CONVEYOR_LEFT,
                                     .ch = '<',
                                     .color = COLOR_PAIR(BLUE_BLACK),
                                     .occupied = true,
                                     .blocks_movement = false,
                                     .movable = false};

tile_t tile_conveyor_right = (tile_t){.id = TILE_CONVEYOR_RIGHT,
                                      .ch = '>',
                                      .color = COLOR_PAIR(BLUE_BLACK),
                                      .occupied = true,
                                      .blocks_movement = false,
                                      .movable = false};

tile_t tile_conveyor_down = (tile_t){.id = TILE_CONVEYOR_DOWN,
                                     .ch = 'V',
                                     .color = COLOR_PAIR(BLUE_BLACK),
                                     .occupied = true,
                                     .blocks_movement = false,
                                     .movable = false};

tile_t tile_letter_w = (tile_t){.id = TILE_LETTER_W,
                                .ch = 'W',
                                .color = COLOR_PAIR(WHITE_BLACK),
                                .occupied = true,
                                .blocks_movement = false,
                                .movable = true};

tile_t tile_letter_i = (tile_t){.id = TILE_LETTER_I,
                                .ch = 'I',
                                .color = COLOR_PAIR(WHITE_BLACK),
                                .occupied = true,
                                .blocks_movement = false,
                                .movable = true};

tile_t tile_letter_n = (tile_t){.id = TILE_LETTER_N,
                                .ch = 'N',
                                .color = COLOR_PAIR(WHITE_BLACK),
                                .occupied = true,
                                .blocks_movement = false,
                                .movable = true};

tile_t tile_player_spawn = (tile_t){.id = TILE_PLAYER_SPAWN,
                                    .ch = '.',
                                    .color = COLOR_PAIR(GREEN_BLACK),
                                    .occupied = true,
                                    .blocks_movement = false,
                                    .movable = false};

//----Levels--------------------------
dyn_array_t g_hole_positions;
dyn_array_t g_conveyor_positions;
vec2_t g_w_tile_pos;

// Level layout loader
internal void load_level_layout(int rows, int cols, int level_array[][cols],
                                char *layout_path) {
  FILE *fh_layout_file;

  fh_layout_file = fopen(layout_path, "r");

  if (fh_layout_file == NULL) {
    fprintf(stderr, "ERROR: couldn't open specified file\n");
    exit(1);
  }

  char buffer[cols + 1];
  memset(buffer, 0, sizeof(buffer));
  for (int y = 0; y < rows; y++) {
    if (fgets(buffer, cols + 1, fh_layout_file) == NULL)
      break;
    if (strlen(buffer) == cols) {
      for (int x = 0; x < cols + 1; x++) {
        switch (buffer[x]) {
        case '0':
          level_array[y][x] = TILE_EMPTY;
          break;
        case '#':
          level_array[y][x] = TILE_WALL;
          break;
        case 'O':
          level_array[y][x] = TILE_ROCK;
          break;
        case 'X':
          level_array[y][x] = TILE_HOLE;
          break;
        case '>':
          level_array[y][x] = TILE_CONVEYOR_RIGHT;
          break;
        case 'V':
          level_array[y][x] = TILE_CONVEYOR_DOWN;
          break;
        case '<':
          level_array[y][x] = TILE_CONVEYOR_LEFT;
          break;
        case '.':
          level_array[y][x] = TILE_PLAYER_SPAWN;
          break;
        case 'W':
          level_array[y][x] = TILE_LETTER_W;
          break;
        case 'I':
          level_array[y][x] = TILE_LETTER_I;
          break;
        case 'N':
          level_array[y][x] = TILE_LETTER_N;
          break;
        }
      }
    } else {
      // Read in some leftover input, don't increment y
      y--;
    }
  }

  fclose(fh_layout_file);
}

internal vec2_t load_level(char *layout_path, tile_map_t map_ground,
                           tile_map_t map_surface) {
  vec2_t player_start_pos = {0, 0};

  g_hole_positions = array_create(4, T_VEC);
  g_conveyor_positions = array_create(4, T_VEC);

  // Load level layout from file
  int level_surface[map_surface.MAP_HEIGHT][map_surface.MAP_WIDTH];
  load_level_layout(map_surface.MAP_HEIGHT, map_surface.MAP_WIDTH,
                    level_surface, layout_path);

  // Place tiles based on layout
  vec2_t pos;
  for (int y = 0; y < map_surface.MAP_HEIGHT; y++) {
    for (int x = 0; x < map_surface.MAP_WIDTH; x++) {
      switch (level_surface[y][x]) {
      case TILE_EMPTY:
        map_surface.p_tiles[y][x] = tile_empty;
        break;
      case TILE_WALL:
        map_surface.p_tiles[y][x] = tile_wall;
        break;
      case TILE_ROCK:
        map_surface.p_tiles[y][x] = tile_rock;
        break;
      case TILE_HOLE:
        map_ground.p_tiles[y][x] = tile_hole;
        pos = (vec2_t){y, x};
        array_push(&g_hole_positions, g_hole_positions.occupied, (void *)&pos);
        break;
      case TILE_CONVEYOR_RIGHT:
        map_ground.p_tiles[y][x] = tile_conveyor_right;
        pos = (vec2_t){y, x};
        array_push(&g_conveyor_positions, g_conveyor_positions.occupied,
                   (void *)&pos);
        break;
      case TILE_CONVEYOR_DOWN:
        map_ground.p_tiles[y][x] = tile_conveyor_down;
        pos = (vec2_t){y, x};
        array_push(&g_conveyor_positions, g_conveyor_positions.occupied,
                   (void *)&pos);
        break;
      case TILE_CONVEYOR_LEFT:
        map_ground.p_tiles[y][x] = tile_conveyor_left;
        pos = (vec2_t){y, x};
        array_push(&g_conveyor_positions, g_conveyor_positions.occupied,
                   (void *)&pos);
        break;
      case TILE_PLAYER_SPAWN:
        map_ground.p_tiles[y][x] = tile_player_spawn;
        player_start_pos = (vec2_t){y, x};
        break;
      case TILE_LETTER_W:
        map_surface.p_tiles[y][x] = tile_letter_w;
        g_w_tile_pos = (vec2_t){y, x};
        break;
      case TILE_LETTER_I:
        map_surface.p_tiles[y][x] = tile_letter_i;
        break;
      case TILE_LETTER_N:
        map_surface.p_tiles[y][x] = tile_letter_n;
        break;
      }
    }
  }

  return player_start_pos;
}

// Somewhat unnecessary intermediate functions ...
internal vec2_t level_test_init(tile_map_t map_ground, tile_map_t map_surface) {
  return load_level("examples/sokoban/levels/level_test_layout.txt", map_ground,
                    map_surface);
}

internal vec2_t level_easy_init(tile_map_t map_ground, tile_map_t map_surface) {
  return load_level("examples/sokoban/levels/level_easy_layout.txt", map_ground,
                    map_surface);
}

internal vec2_t level_medium_init(tile_map_t map_ground,
                                  tile_map_t map_surface) {
  return load_level("examples/sokoban/levels/level_medium_layout.txt",
                    map_ground, map_surface);
}

internal vec2_t level_hard_init(tile_map_t map_ground, tile_map_t map_surface) {
  return load_level("examples/sokoban/levels/level_hard_layout.txt", map_ground,
                    map_surface);
}

vec2_t soko_level_init(int level_id, tile_map_t map_ground,
                       tile_map_t map_surface) {

  switch (level_id) {
  case LEVEL_TEST:
    return level_test_init(map_ground, map_surface);
  case LEVEL_EASY:
    return level_easy_init(map_ground, map_surface);
  case LEVEL_MEDIUM:
    return level_medium_init(map_ground, map_surface);
  case LEVEL_HARD:
    return level_hard_init(map_ground, map_surface);
  }

  return (vec2_t){1, 1};
}
