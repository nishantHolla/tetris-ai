/*
   Testing SHA256 hash generation of all possible start pieces and rotations
*/

#include "tetris.h"

int main(void) {
  tet_Game game;

  if (tet_game_init(&game, &tet_default_chromosome) != 0) {
    perror("Failed to initialize board.\n");
    exit(1);
  }

  for (int32_t i = 0; i < NUMBER_OF_PIECES; i++) {
    game.current_piece.index = i;
    for (int32_t j = 0; j < NUMBER_OF_ROTATIONS; j++) {
      game.current_piece.rotation = j;
      tet_debug_print_game(&game, true);
    }
  }

  return 0;
}
