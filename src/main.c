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
    BeginDrawing();
    ClearBackground(TET_UI_WINDOW_BG_COLOR);
    tet_ui_calculate();
    tet_ui_draw_text();
    tet_ui_draw_board(&game);
    EndDrawing();
  }

  // Clean game

  CloseWindow();
  tet_ui_free_fonts();
  return 0;
}
