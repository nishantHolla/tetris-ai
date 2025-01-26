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

bool tet_game_can_move(tet_Game game, const tet_Move move) {
  /*
     Checks if current_piece of given game can move in the given direction. Returns true if it can
     move else returns false
  */

  if (!tet_game_is_valid(&game)) {
    return false;
  }

  switch (move) {
    case TET_MOVE_DOWN:
      game.position.y--;
      break;
    case TET_MOVE_LEFT:
      game.position.x--;
      break;
    case TET_MOVE_RIGHT:
      game.position.x++;
      break;
    case TET_MOVE_CLOCKWISE:
      game.current_piece.rotation = ROTATE_CLOCKWISE(game.current_piece);
      break;
    case TET_MOVE_ANTI_CLOCKWISE:
      game.current_piece.rotation = ROTATE_ANTI_CLOCKWISE(game.current_piece);
      break;
    case TET_MOVE_DROP:
      return true;
    case TET_MOVE_SWAP:
      return !game.has_swapped;
    default:
      return false;
  }

  return tet_game_is_valid(&game);
}

int8_t tet_game_move(tet_Game *game, const tet_Move move) {
  /*
     Moves the current_piece of given game in the given direction. Returns 0 if successful else returns
     non 0 integer
  */

  if (!tet_game_can_move(*game, move)) {
    return 1;
  }

  switch (move) {
    case TET_MOVE_DOWN:
      game->position.y--;
      break;
    case TET_MOVE_LEFT:
      game->position.x--;
      break;
    case TET_MOVE_RIGHT:
      game->position.x++;
      break;
    case TET_MOVE_CLOCKWISE:
      game->current_piece.rotation = ROTATE_CLOCKWISE(game->current_piece);
      break;
    case TET_MOVE_ANTI_CLOCKWISE:
      game->current_piece.rotation = ROTATE_ANTI_CLOCKWISE(game->current_piece);
      break;
    case TET_MOVE_DROP:
      while (tet_game_can_move(*game, TET_MOVE_DOWN)) {
        game->position.y--;
      }
      break;
    case TET_MOVE_SWAP:
      tet_Piece temp = game->current_piece;
      game->has_swapped = true;

      if (game->has_held_piece) {
        game->current_piece = game->held_piece;
      }
      else {
        game->current_piece = game->next_piece;
        game->next_piece = tet_generate_random_piece();
        game->has_held_piece = true;
      }
      game->held_piece = temp;
      game->position = START_POSITION;
      break;
    default:
      return 2;
  }

  return 0;
}

bool tet_game_is_valid(const tet_Game *game) {
  /*
     Check if given game is in valid state. Returns true if it is else returns false
  */

  const int32_t px = game->position.x;
  const int32_t py = game->position.y;
  const uint16_t shape = TET_PIECE_SHAPE[game->current_piece.index][game->current_piece.rotation];

  for (int32_t row = 0; row < 4; row++) {
    for (int32_t col = 0; col < 4; col++) {
      if (SHAPE_BIT(shape,row,col) &&
          (py+row < 0 || py+row >= GRID_ROW_COUNT ||
           px+col < 0 || px+col >= GRID_COL_COUNT ||
           game->board[py+row][px+col] != GRID_EMPTY_CELL)) {
        return false;
      }
    }
  }

  return true;
}
