#include "tetris.h"

int main(void) {
  srand(time(NULL));

  // Init game

  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(GetScreenWidth(), GetScreenHeight(), "Tetris AI");
  SetTargetFPS(60);

  tet_ui_init_fonts();
  tet_Game game;
  if (tet_game_init(&game) != 0) {
    perror("Failed to initialize game.\n");
    exit(1);
  }

  double last_call_time = 0;
  const double last_call_interval = AI_IS_PLAYING ? 0.1 : 0.3;

  // Game loop

  while (!WindowShouldClose()) {

    double current_time = GetTime();
    if (!game.is_over && current_time - last_call_time >= last_call_interval) {
      if (AI_IS_PLAYING) {
        tet_game_play(&game);
      }
      else if (tet_game_move(&game, TET_MOVE_DOWN) != 0) {
        tet_game_place(&game);
        tet_game_end_turn(&game);
      }
      last_call_time = current_time;
    }

    if (!AI_IS_PLAYING && !game.is_over) {
      if (IsKeyPressed(KEY_LEFT)) {
        tet_game_move(&game, TET_MOVE_LEFT);
      }
      else if (IsKeyPressed(KEY_RIGHT)) {
        tet_game_move(&game, TET_MOVE_RIGHT);
      }
      else if (IsKeyPressed(KEY_UP)) {
        tet_game_move(&game, TET_MOVE_ANTI_CLOCKWISE);
      }
      else if (IsKeyPressed(KEY_DOWN)) {
        tet_game_move(&game, TET_MOVE_CLOCKWISE);
      }
      else if (IsKeyPressed(KEY_SPACE)) {
        tet_game_move(&game, TET_MOVE_DROP);
        tet_game_place(&game);
        tet_game_end_turn(&game);
      }
      else if (IsKeyPressed(KEY_F)) {
        tet_game_move(&game, TET_MOVE_SWAP);
      }
    }

    if (game.is_over) {
      if (IsKeyPressed(KEY_R)) {
        tet_game_init(&game);
        continue;
      }
    }

    BeginDrawing();

    ClearBackground(TET_UI_WINDOW_BG_COLOR);
    tet_ui_calculate(&game);
    tet_ui_draw_text(&game);
    tet_ui_draw_board(&game);
    tet_ui_draw_next_and_held_piece(&game);

    EndDrawing();
  }

  // Clean game

  CloseWindow();
  tet_ui_free_fonts();
  return 0;
}
