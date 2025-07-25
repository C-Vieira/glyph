#include "../snake.h"

static void snake_register_scenes() {
  // Scenes Init
  // Title
  gp_snake_title_scene = scene_create();

  gp_snake_title_scene->init = snake_title_init;
  gp_snake_title_scene->handle_input = snake_title_handle_input;
  gp_snake_title_scene->update = snake_title_update;
  gp_snake_title_scene->draw = snake_title_draw;
  gp_snake_title_scene->shutdown = snake_title_shutdown;

  // Game
  gp_snake_game_scene = scene_create();

  gp_snake_game_scene->init = snake_game_init;
  gp_snake_game_scene->handle_input = snake_game_handle_input;
  gp_snake_game_scene->update = snake_game_update;
  gp_snake_game_scene->draw = snake_game_draw;
  gp_snake_game_scene->shutdown = snake_game_shutdown;
}

// Client Entry Point
game_data_t glyph_main() {

  snake_register_scenes();

  // Return game data to framework main
  return (game_data_t){.should_close = false,
                       .curr_scene = gp_snake_title_scene};
}
