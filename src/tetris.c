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

  game->is_over = false;
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

bool tet_game_can_place(const tet_Game *game) {
  /*
     Checks if current_piece can be placed in the current position. Returns true if it can else returns
     false
  */

  return tet_game_is_valid(game) && !tet_game_can_move(*game, TET_MOVE_DOWN);
}

int8_t tet_game_place(tet_Game *game) {
  /*
     Places the current_piece of the given game onto the board. Returns 0 if successful else returns
     non 0 integer
  */

  if (!tet_game_can_place(game)) {
    return 1;
  }

  const uint16_t shape = TET_PIECE_SHAPE[game->current_piece.index][game->current_piece.rotation];
  const int32_t px = game->position.x;
  const int32_t py = game->position.y;

  for (int32_t row = 0; row < 4; row++) {
    for (int32_t col = 0; col < 4; col++) {
      if (SHAPE_BIT(shape, row, col)) {
        game->board[py+row][px+col] = game->current_piece.index;
      }
    }
  }

  tet_game_calculate_heights(game);
  tet_game_calculate_holes(game);
  tet_game_calculate_bumpiness(game);

  return 0;
}

int8_t tet_game_end_turn(tet_Game *game) {
  /*
     Performs necessary actions to end a given turn. Returns 0 if successful else return non 0 integer
  */

  game->current_piece = game->next_piece;
  game->next_piece = tet_generate_random_piece();
  game->has_swapped = false;
  game->position = START_POSITION;

  if (!tet_game_is_valid(game)) {
    game->is_over = true;
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

int8_t tet_game_calculate_heights(tet_Game *game) {
  /*
     Calculates heights of each column and updates the heights parameter of given game. Returns 0
     if successful else returns non 0 integer
  */

  for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
    int8_t max_height = 0;
    for (int32_t row = 0; row < GRID_ROW_COUNT; row++) {
      if (game->board[row][col] != GRID_EMPTY_CELL) {
        max_height = row+1;
      }
    }
    game->heights[col] = max_height;
  }

  return 0;
}

int8_t tet_game_calculate_holes(tet_Game *game) {
  /*
     Calculates total holes in the given game and updates the holes parameter of the game struct. Returns
     0 if successful else returns non 0 integer. This functions assumes that the heights parameter
     of the given game struct is updated before and has accurate values
  */

  int32_t hole_count = 0;

  for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
    for (int32_t row = 0; row < game->heights[col]; row++) {
      if (game->board[row][col] == GRID_EMPTY_CELL) {
        hole_count += (col == 0 || col == GRID_COL_COUNT - 1) ? 2 : 1;
      }
    }
  }

  game->holes = hole_count;
  return 0;
}

int8_t tet_game_calculate_bumpiness(tet_Game *game) {
  /*
     Calculates the bumpiness of the surface of the given game and updates the bumpiness parameter of
     the game struct. Returns 0 if successful else returns non 0 integer. This function assumes that the
     heights parameter of the given game struct is updated before and has accurate values
  */

  int32_t bumpiness = 0;

  for (int32_t col = 0; col < GRID_COL_COUNT - 1; col++) {
    bumpiness += abs(game->heights[col] - game->heights[col+1]);
  }

  game->bumpiness = bumpiness;
  return 0;
}
