#include "tetris.h"

int main(void) {
  srand(time(NULL));

  tet_Game game;

  if (tet_game_init(&game) != 0) {
    perror("Failed to initialize game.\n");
    exit(1);
  }

  tet_debug_print_game(&game, true);

  return 0;
}
