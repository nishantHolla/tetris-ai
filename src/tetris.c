#include "tetris.h"

// Game Values

const tet_Position START_POSITION = {
  .x = GRID_COL_COUNT / 2 - 2,
  .y = GRID_ROW_COUNT - 4
};

const uint16_t TET_PIECE_SHAPE[NUMBER_OF_PIECES][NUMBER_OF_ROTATIONS] = {
  { 0b0000111100000000, 0b0100010001000100, 0b0000000011110000, 0b0010001000100010 }, // TET_PIECE_I
  { 0b0100011100000000, 0b0010001001100000, 0b0000011100010000, 0b0011001000100000 }, // TET_PIECE_L
  { 0b0001011100000000, 0b0110001000100000, 0b0000011101000000, 0b0010001000110000 }, // TET_PIECE_LR
  { 0b0011001100000000, 0b0011001100000000, 0b0011001100000000, 0b0011001100000000 }, // TET_PIECE_O
  { 0b0110001100000000, 0b0010011001000000, 0b0000011000110000, 0b0001001100100000 }, // TET_PIECE_S
  { 0b0011011000000000, 0b0100011000100000, 0b0000001101100000, 0b0010001100010000 }, // TET_PIECE_SR
  { 0b0010011100000000, 0b0010011000100000, 0b0000011100100000, 0b0010001100100000 }, // TET_PIECE_T
};

// Game Functions

int8_t tet_game_init(tet_Game *game) {
  /*
     Initializes the given game struct. Returns 0 on successful initialization else returns non 0
     integer
  */

  for (int32_t row = 0; row < GRID_ROW_COUNT; row++) {
    for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
      game->board[row][col] = GRID_EMPTY_CELL;
    }
  }

  game->position = START_POSITION;
  game->current_piece = tet_generate_random_piece();
  game->next_piece = tet_generate_random_piece();
  game->held_piece = tet_generate_random_piece();
  game->has_held_piece = false;
  game->has_swapped = false;
  game->score = 0;

  return 0;
}
