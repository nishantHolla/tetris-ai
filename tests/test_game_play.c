/*
   Test first 100 moves
*/

#include "tetris.h"

int main(void) {
  srand(time(NULL));
  tet_Game game;

  if (tet_game_init(&game, &tet_default_chromosome) != 0) {
    perror("Failed to initialze game.\n");
    exit(1);
  }

  for (int i = 0; i < 100 && !game.is_over; i++) {
    tet_game_play(&game);
    if (game.last_lines_cleared == 4) {
      debug_4lc_count++;
    }
    else if (game.last_lines_cleared == 3) {
      debug_3lc_count++;
    }
    else if (game.last_lines_cleared == 2) {
      debug_2lc_count++;
    }
    else if (game.last_lines_cleared == 1) {
      debug_1lc_count++;
    }
  }
  printf("debug_4lc_count: %d\n", debug_4lc_count);
  printf("debug_3lc_count: %d\n", debug_3lc_count);
  printf("debug_2lc_count: %d\n", debug_2lc_count);
  printf("debug_1lc_count: %d\n", debug_1lc_count);
  return 0;
}
