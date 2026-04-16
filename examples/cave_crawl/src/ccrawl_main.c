#include "../ccrawl.h"

internal void ccrawl_register_scenes() {
  // Game
  gp_ccrawl_game_scene = scene_create();

  gp_ccrawl_game_scene->c_init = ccrawl_game_init;
  gp_ccrawl_game_scene->c_handle_input = ccrawl_game_handle_input;
  gp_ccrawl_game_scene->c_update = ccrawl_game_update;
  gp_ccrawl_game_scene->c_draw = ccrawl_game_draw;
  gp_ccrawl_game_scene->c_shutdown = ccrawl_game_shutdown;
}

game_data_t glyph_main() {
  ccrawl_register_scenes();

  return (game_data_t){.should_close = false,
                       .p_curr_scene = gp_ccrawl_game_scene};
}
