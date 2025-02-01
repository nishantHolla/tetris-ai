/*
   Test 1st move generation
*/

#include "tetris.h"

int main(void) {
  srand(time(NULL));
  tet_Game game;
  tet_HashMap map;

  if (tet_game_init(&game) != 0) {
    perror("Failed to initialze game.\n");
    exit(1);
  }
  if (tet_hashmap_init(&map) != 0) {
    perror("Failed to initialze hashmap.\n");
    exit(1);
  }

  tet_game_calculate(game, &map);
  tet_hashmap_free(&map);
  return 0;
}
