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

  // Game loop

  while (!WindowShouldClose()) {
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
    }
    else if (IsKeyPressed(KEY_F)) {
      tet_game_move(&game, TET_MOVE_SWAP);
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
