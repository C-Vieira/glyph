#include "../snake.h"

scene_data_t *gp_snake_game_scene;

// Static global score reference
static int s_score;

// Static global views
static view_data_t *sp_game_board;
static view_data_t *sp_score_board;
static view_data_t *sp_pause_screen;
static view_data_t *sp_game_over_screen;

// Global player reference (could also be static)
snake_player_t g_player;

// Static global fruit reference
static entity_t s_fruit;

// Helper Functions
internal void snake_game_pause() {
  // Paused State
  view_draw(sp_pause_screen);
  view_draw_message_at(sp_pause_screen, 2, 6, "PAUSED");
  view_refresh(sp_pause_screen);
  while (view_get_input(sp_game_board) != 'p')
    ;
  view_clear(sp_pause_screen);
}

internal void snake_game_over() {
  // Game Over State
  view_draw(sp_game_over_screen);
  view_draw_message_at(sp_game_over_screen, 2, 1, "   GAME OVER!   ");
  view_draw_message_at(sp_game_over_screen, 3, 1, ">'r' to restart<");
  view_refresh(sp_game_over_screen);
  while (view_get_input(sp_game_board) != 'r')
    ;
  view_clear(sp_game_over_screen);
}

internal void snake_game_start() {
  // Clear main view
  view_clear(sp_game_board);

  // Setup score board
  s_score = 0;
  view_draw_message_at(sp_score_board, 1, 1, "Score: 0      ");
  view_draw(sp_score_board);

  // Player Init
  // Create the player entity
  entity_t player = {
      .dir = {1, 0}, // Down
  };

  // In case of restart
  if (g_player.p_segments != NULL) {
    queue_free(g_player.p_segments);
  }

  g_player =
      (snake_player_t){.player = player, .p_segments = queue_create(T_ENTITY)};
  snake_add_segment(&g_player, (vec2_t){2, 2});
  view_draw_entity_at(sp_game_board, snake_get_head(&g_player));

  // Create initial segments
  for (int i = 0; i < 4; i++) {
    entity_t player = g_player.player;

    vec2_t next_pos = vector_add(snake_get_head(&g_player)->pos, player.dir);

    snake_add_segment(&g_player, next_pos);
    view_draw_entity_at(sp_game_board, snake_get_head(&g_player));
  }

  // Fruit Init
  // Create the fruit entity
  s_fruit = (entity_t){
      .pos = {0, 0}, .dir = {0, 0}, .ch = 'o', .color = COLOR_PAIR(RED_BLACK)};
  // Place at a random spot on the board
  view_get_empty_coords(sp_game_board, &s_fruit.pos.y, &s_fruit.pos.x);
  // Draw it on the screen
  view_draw_entity_at(sp_game_board, &s_fruit);

  // Draw view and refresh
  view_draw(sp_game_board);
}

void snake_game_init() {
  // Views Init
  sp_pause_screen = view_create(5, 18, 0, 0);
  sp_game_over_screen = view_create(5, 18, 0, 0);

  // Create and set main view
  sp_game_board = view_create(28, 50, 0, 0);
  view_set_timeout(sp_game_board, 300);
  gp_snake_game_scene->p_main_view = sp_game_board;

  // Create score board
  sp_score_board =
      view_create(3, sp_game_board->width, sp_game_board->height + 3, 0);

  // Start Game
  snake_game_start();
}

void snake_game_handle_input() {
  chtype input = view_get_input(sp_game_board);
  if (input == 'q') {
    // Transition to title scene
    game_change_scene(gp_snake_title_scene);
  }

  vec2_t new_dir;
  switch (input) {
  case 'w': {
    new_dir = VEC_UP;
    break;
  }
  case 'a': {
    new_dir = VEC_LEFT;
    break;
  }
  case 's': {
    new_dir = VEC_DOWN;
    break;
  }
  case 'd': {
    new_dir = VEC_RIGHT;
    break;
  }
  case 'p': {
    // Pause
    snake_game_pause();
  } // Fall through
  default:
    new_dir = g_player.player.dir;
  }

  // Ignore invalid moves
  if (!vector_equals(vector_add(g_player.player.dir, new_dir), VEC_ZERO))
    g_player.player.dir = new_dir;
}

void snake_game_update() {
  // Update player position
  vec2_t new_pos;
  entity_t *head = snake_get_head(&g_player);

  new_pos = vector_add(g_player.player.dir, head->pos);

  // Handle Collisions
  if (view_at_border(sp_game_board, new_pos) ||
      view_get_char_at(sp_game_board, new_pos) ==
          (snake_get_head(&g_player)->ch | snake_get_head(&g_player)->color)) {

    snake_game_over();
    // Reset Game
    snake_game_start();
    return;
  }

  // Collect fruit
  if (vector_equals(new_pos, s_fruit.pos)) {
    s_score += 10;
    view_get_empty_coords(sp_game_board, &s_fruit.pos.y, &s_fruit.pos.x);
    snake_add_segment(&g_player, new_pos);
  } else {
    // Move
    //  Clear trail
    entity_t *tail = snake_get_tail(&g_player);
    view_clear_char_at(sp_game_board, tail->pos);

    snake_move_to(&g_player, new_pos);
  }
}

void snake_game_draw() {
  // Draw fruit
  view_draw_entity_at(sp_game_board, &s_fruit);

  // Draw player
  view_draw_entity_at(sp_game_board, snake_get_head(&g_player));

  view_draw_num_at(sp_score_board, 1, 8, s_score);
  view_refresh(sp_score_board);

  view_refresh(sp_game_board);
}

void snake_game_shutdown() {
  // Delete player
  queue_free(g_player.p_segments);
  g_player.p_segments = NULL;

  // Delete window from main game view
  delwin(sp_game_board->p_view_window);

  // Delete other windows from views
  delwin(sp_score_board->p_view_window);
  delwin(sp_pause_screen->p_view_window);
  delwin(sp_game_over_screen->p_view_window);

  // Free allocated memory for views
  free(sp_game_board);

  free(sp_score_board);
  free(sp_pause_screen);
  free(sp_game_over_screen);
}
