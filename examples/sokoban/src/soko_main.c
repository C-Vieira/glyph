#include "../soko.h"

internal void soko_register_scenes() {
  // Scenes Init
  // Title
  gp_soko_title_scene = scene_create();

  gp_soko_title_scene->c_init = soko_title_init;
  gp_soko_title_scene->c_handle_input = soko_title_handle_input;
  gp_soko_title_scene->c_update = soko_title_update;
  gp_soko_title_scene->c_draw = soko_title_draw;
  gp_soko_title_scene->c_shutdown = soko_title_shutdown;

  // Level Select
  gp_soko_lvlselect_scene = scene_create();

  gp_soko_lvlselect_scene->c_init = soko_lvlselect_init;
  gp_soko_lvlselect_scene->c_handle_input = soko_lvlselect_handle_input;
  gp_soko_lvlselect_scene->c_update = soko_lvlselect_update;
  gp_soko_lvlselect_scene->c_draw = soko_lvlselect_draw;
  gp_soko_lvlselect_scene->c_shutdown = soko_lvlselect_shutdown;

  // Game
  gp_soko_game_scene = scene_create();

  gp_soko_game_scene->c_init = soko_game_init;
  gp_soko_game_scene->c_handle_input = soko_game_handle_input;
  gp_soko_game_scene->c_update = soko_game_update;
  gp_soko_game_scene->c_draw = soko_game_draw;
  gp_soko_game_scene->c_shutdown = soko_game_shutdown;
}

game_data_t glyph_main() {
  soko_register_scenes();

  return (game_data_t){.should_close = false,
                       .p_curr_scene = gp_soko_title_scene};
}
