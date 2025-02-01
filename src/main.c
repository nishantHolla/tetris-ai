#include "tetris.h"

int main(void) {
  srand(time(NULL));
  tet_Game game;

  if (tet_game_init(&game) != 0) {
    perror("Failed to initialze game.\n");
    exit(1);
  }

  for (int i = 0; i < 10000; i++) {
    tet_game_play(&game);
  }
  return 0;
}
