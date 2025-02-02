/*
   Test set, remove and get functions of hashmap
*/

#include "tetris.h"

int main(void) {
  tet_Game game;
  tet_HashMap map;

  if (tet_game_init(&game, &tet_default_chromosome) != 0) {
    perror("Failed to initialize game.\n");
    exit(1);
  }
  if (tet_hashmap_init(&map) != 0) {
    perror("Failed to initialize map.\n");
    exit(1);
  }

  double value = 0;
  for (int32_t i = 0; i < NUMBER_OF_PIECES; i++) {
    game.current_piece.index = i;
    for (int32_t j = 0; j < NUMBER_OF_ROTATIONS; j++) {
      game.current_piece.rotation = j;
      tet_hashmap_set(&map, &game, value);
      value += 10;
    }
  }

  game.current_piece.index = 4;
  game.current_piece.rotation = 2;
  tet_hashmap_remove(&map, &game);

  while (1) {
    int32_t a, b;
    scanf("%d %d", &a, &b);
    if (b == -1 || a == -1) {
      break;
    }

    game.current_piece.index = a;
    game.current_piece.rotation = b;

    double value = 0;
    tet_hashmap_get(&map, &game, &value);
    printf("%.2f\n", value);
  }

  tet_hashmap_free(&map);
  return 0;
}
