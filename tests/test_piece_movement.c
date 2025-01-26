/*
   Test the movement of all pieces
*/

#include "tetris.h"

int main(void) {
  srand(time(NULL));
  tet_Game game;

  if (tet_game_init(&game) != 0) {
    perror("Failed to initialze game.\n");
    exit(1);
  }

  // Test Down Movement

  for (int32_t i = 0; i < NUMBER_OF_PIECES; i++) {
    game.current_piece.index = i;
    for (int32_t j = 0; j < NUMBER_OF_ROTATIONS; j++) {
      game.current_piece.rotation = j;
      game.position = START_POSITION;
      tet_debug_print_game(&game, true);
      while (tet_game_can_move(game, TET_MOVE_DOWN)) {
        tet_game_move(&game, TET_MOVE_DOWN);
        tet_debug_print_game(&game, true);
      }
    }
  }

  // Test Left Movement

  for (int32_t i = 0; i < NUMBER_OF_PIECES; i++) {
    game.current_piece.index = i;
    for (int32_t j = 0; j < NUMBER_OF_ROTATIONS; j++) {
      game.current_piece.rotation = j;
      game.position = START_POSITION;
      tet_debug_print_game(&game, true);
      while (tet_game_can_move(game, TET_MOVE_LEFT)) {
        tet_game_move(&game, TET_MOVE_LEFT);
        tet_debug_print_game(&game, true);
      }
    }
  }

  // Test Right Movement

  for (int32_t i = 0; i < NUMBER_OF_PIECES; i++) {
    game.current_piece.index = i;
    for (int32_t j = 0; j < NUMBER_OF_ROTATIONS; j++) {
      game.current_piece.rotation = j;
      game.position = START_POSITION;
      tet_debug_print_game(&game, true);
      while (tet_game_can_move(game, TET_MOVE_LEFT)) {
        tet_game_move(&game, TET_MOVE_LEFT);
        tet_debug_print_game(&game, true);
      }
    }
  }

  // Test Clockwise Rotation

  for (int32_t i = 0; i < NUMBER_OF_PIECES; i++) {
    game.current_piece.index = i;
    for (int32_t j = 0; j < NUMBER_OF_ROTATIONS; j++) {
      game.position = START_POSITION;
      tet_game_move(&game, TET_MOVE_CLOCKWISE);
      tet_debug_print_game(&game, true);
    }
  }

  // Test Anti Clockwise Rotation

  for (int32_t i = 0; i < NUMBER_OF_PIECES; i++) {
    game.current_piece.index = i;
    for (int32_t j = 0; j < NUMBER_OF_ROTATIONS; j++) {
      game.position = START_POSITION;
      tet_game_move(&game, TET_MOVE_ANTI_CLOCKWISE);
      tet_debug_print_game(&game, true);
    }
  }

  // Test Dropping

  for (int32_t i = 0; i < NUMBER_OF_PIECES; i++) {
    game.current_piece.index = i;
    for (int32_t j = 0; j < NUMBER_OF_ROTATIONS; j++) {
      game.position = START_POSITION;
      game.current_piece.rotation = j;
      tet_game_move(&game, TET_MOVE_DROP);
      tet_debug_print_game(&game, true);
    }
  }

  // Test Swapping

  game.position = START_POSITION;
  tet_debug_print_game(&game, true);
  tet_game_move(&game, TET_MOVE_SWAP);
  tet_debug_print_game(&game, true);
  tet_game_move(&game, TET_MOVE_SWAP);
  tet_debug_print_game(&game, true);
  return 0;
}
