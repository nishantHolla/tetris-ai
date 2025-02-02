/*
   Test piece placement and turn ending
*/

#include "tetris.h"

int main(void) {
  srand(time(NULL));
  tet_Game game;

  if (tet_game_init(&game, &tet_default_chromosome) != 0) {
    perror("Failed to initialze game.\n");
    exit(1);
  }

  while (!game.is_over) {
    tet_game_move(&game, TET_MOVE_DROP);
    tet_game_place(&game);
    tet_game_end_turn(&game);
    tet_debug_print_game(&game, true);
  }

  return 0;
}
