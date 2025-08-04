#include "../snake.h"

internal void snake_register_scenes() {
  // Scenes Init
  // Title
  gp_snake_title_scene = scene_create();

  gp_snake_title_scene->c_init = snake_title_init;
  gp_snake_title_scene->c_handle_input = snake_title_handle_input;
  gp_snake_title_scene->c_update = snake_title_update;
  gp_snake_title_scene->c_draw = snake_title_draw;
  gp_snake_title_scene->c_shutdown = snake_title_shutdown;

  // Game
  gp_snake_game_scene = scene_create();

  gp_snake_game_scene->c_init = snake_game_init;
  gp_snake_game_scene->c_handle_input = snake_game_handle_input;
  gp_snake_game_scene->c_update = snake_game_update;
  gp_snake_game_scene->c_draw = snake_game_draw;
  gp_snake_game_scene->c_shutdown = snake_game_shutdown;
}

// Client Entry Point
game_data_t glyph_main() {

  snake_register_scenes();

  // Return game data to framework main
  return (game_data_t){.should_close = false,
                       .p_curr_scene = gp_snake_title_scene};
}
