/*
   Test 2nd move generation
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

  game.current_piece.index = TET_PIECE_SR;
  game.current_piece.rotation = TET_ROTATION_EAST;

  tet_game_move(&game, TET_MOVE_DROP);
  tet_game_place(&game);
  tet_game_end_turn(&game);

  game.current_piece.index = TET_PIECE_LR;
  tet_MoveList best_moves = { .buffer = {0}, .size = 0 };
  tet_MoveList moves = { .buffer = {0}, .size = 0 };
  tet_PiecePlacement best_placement;
  double best_evaluation = -DBL_MAX;

  tet_game_calculate(game, &map, &best_evaluation, &best_placement, &best_moves, &moves);
  printf("Best Evaluation: %.2f\n", best_evaluation);
  tet_hashmap_free(&map);

  return 0;
}
