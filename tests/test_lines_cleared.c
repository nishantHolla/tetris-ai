/*
   Test filled lines clearing logic
*/

#include "tetris.h"

int main(void) {
  srand(time(NULL));
  tet_Game game;

  if (tet_game_init(&game) != 0) {
    perror("Failed to initialze game.\n");
    exit(1);
  }

  for (int32_t row = 0; row < 2; row++) {
    for (int32_t col = 0; col < GRID_COL_COUNT - 1; col++) {
      game.board[row][col] = 1;
    }
  }

  game.current_piece.index = TET_PIECE_I;
  game.current_piece.rotation = TET_ROTATION_EAST;
  while (tet_game_move(&game, TET_MOVE_RIGHT) == 0);
  tet_game_move(&game, TET_MOVE_DROP);
  tet_debug_print_game(&game, true);
  tet_game_place(&game);
  tet_game_end_turn(&game);
  tet_debug_print_game(&game, true);

  return 0;
}
