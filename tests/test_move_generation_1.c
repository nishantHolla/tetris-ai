/*
   Test 1st move generation
*/

#include "tetris.h"

int main(void) {
  srand(time(NULL));
  tet_Game game;
  tet_HashMap map;

  if (tet_game_init(&game, &tet_default_chromosome) != 0) {
    perror("Failed to initialze game.\n");
    exit(1);
  }
  if (tet_hashmap_init(&map) != 0) {
    perror("Failed to initialze hashmap.\n");
    exit(1);
  }

  tet_MoveList best_moves = { .buffer = {0}, .size = 0 };
  tet_MoveList moves = { .buffer = {0}, .size = 0 };
  tet_PiecePlacement best_placement;
  double best_evaluation = -DBL_MAX;

  tet_game_calculate(game, &map, &best_evaluation, &best_placement, &best_moves, &moves);
  tet_hashmap_free(&map);
  return 0;
}
