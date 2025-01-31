/*
   Test calculation of board parameters when piece is placed
*/

#include "tetris.h"

int main(void) {
  srand(time(NULL));
  tet_Game game;

  if (tet_game_init(&game) != 0) {
    perror("Failed to initialze game.\n");
    exit(1);
  }

  tet_game_calculate_heights(&game);
  tet_debug_print_game(&game, true);

  tet_game_move(&game, TET_MOVE_DROP);
  tet_game_place(&game);
  tet_game_end_turn(&game);
  tet_debug_print_game(&game, true);

  for (int32_t i = 0; i < 3; i++) {
    tet_game_move(&game, TET_MOVE_LEFT);
  }
  tet_game_move(&game, TET_MOVE_DROP);
  tet_game_place(&game);
  tet_game_end_turn(&game);
  tet_debug_print_game(&game, true);

  for (int32_t i = 0; i < 3; i++) {
    tet_game_move(&game, TET_MOVE_RIGHT);
  }
  tet_game_move(&game, TET_MOVE_DROP);
  tet_game_place(&game);
  tet_game_end_turn(&game);
  tet_debug_print_game(&game, true);

  return 0;
}
