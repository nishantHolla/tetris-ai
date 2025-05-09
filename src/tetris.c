#include "tetris.h"

// Game Values

bool AI_IS_PLAYING = 1;
bool AI_IS_TRAINING = 0;

const tet_Position START_POSITION = {
  .x = GRID_COL_COUNT / 2 - 1,
  .y = GRID_ROW_COUNT - 4
};

const uint16_t TET_PIECE_SHAPE[NUMBER_OF_PIECES][NUMBER_OF_ROTATIONS] = {
  { 0x0F00, 0x4444, 0x00F0, 0x2222 }, // TET_PIECE_I
  { 0x4700, 0x2260, 0x0710, 0x3220 }, // TET_PIECE_L
  { 0x1700, 0x6220, 0x0740, 0x2230 }, // TET_PIECE_LR
  { 0x3300, 0x3300, 0x3300, 0x3300 }, // TET_PIECE_O
  { 0x6300, 0x2640, 0x0630, 0x1320 }, // TET_PIECE_S
  { 0x3600, 0x4620, 0x0360, 0x2310 }, // TET_PIECE_SR
  { 0x2700, 0x2620, 0x0720, 0x2320 }, // TET_PIECE_T
};

const tet_Coefficient coeff_of_line_clear[5] = {
  { .magnitude = 0, .is_positive = true },
  { .magnitude = 1, .is_positive = true },
  { .magnitude = 1, .is_positive = true },
  { .magnitude = 1, .is_positive = true },
  { .magnitude = 1, .is_positive = true  }
};
const tet_Coefficient coeff_of_bumpiness = { .magnitude = 1, .is_positive = false };
const tet_Coefficient coeff_of_holes = { .magnitude = 1, .is_positive = false };
const tet_Coefficient coeff_of_height = { .magnitude = 1, .is_positive = false };

const tet_Chromosome tet_default_chromosome = {
  .coeff_of_line_clear = {
    { .magnitude = 485.460061, .is_positive = false},
    { .magnitude = 793.878512, .is_positive = false },
    { .magnitude = 762.678770, .is_positive = false },
    { .magnitude = 47.741757, .is_positive = true },
    { .magnitude = 356.161961, .is_positive = true  }
  },
  .coeff_of_holes = { .magnitude = 142.888781, .is_positive = false },
  .coeff_of_height = { .magnitude = 999.241182, .is_positive = false },
  .coeff_of_bumpiness = { .magnitude = 51.685152, .is_positive = false },
};

// Game Functions

int8_t tet_game_init(tet_Game *game, const tet_Chromosome *chromosome) {
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
  game->l4_clear = 0;
  game->l3_clear = 0;
  game->l2_clear = 0;
  game->l1_clear = 0;
  game->chromosome = *chromosome;

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

  tet_Piece temp;
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
      temp = game->current_piece;
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

  tet_game_calculate_lines_removed(game);
  tet_game_calculate_score(game);
  tet_game_calculate_heights(game);
  tet_game_calculate_holes(game);
  tet_game_calculate_bumpiness(game);

  return 0;
}

int8_t tet_game_remove_filled_lines(tet_Game *game) {
  /*
     Removes filled lines from the given game. Returns the number of lines removed
  */

  int8_t lines_removed = 0;

  for (int32_t row = 0; row < GRID_ROW_COUNT; row++) {
    bool line_is_full = true;
    for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
      if (game->board[row][col] == GRID_EMPTY_CELL) {
        line_is_full = false;
        break;
      }
    }

    if (line_is_full) {
      lines_removed++;
      for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
        for (int32_t i = 0; i < GRID_ROW_COUNT - 1; i++) {
          game->board[i][col] = game->board[i+1][col];
        }
        game->board[GRID_ROW_COUNT-1][col] = GRID_EMPTY_CELL;
      }
      row--;
    }
  }

  return lines_removed;
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

int8_t tet_game_calculate_lines_removed(tet_Game *game) {
  /*
     Calculates the number of lines that can be removed and updates the last_lines_cleared parameter
     of given game. Returns 0 if successful else returns non 0 integer
  */

  game->last_lines_cleared = tet_game_remove_filled_lines(game);
  return 0;
}

int8_t tet_game_calculate_score(tet_Game *game) {
  /*
     Calculates the score based on last_lines_cleared of the given game. Assumes that
     tet_game_calculate_lines_removed has been called before running this function. Returns 0
     if successful else returns non 0 integer
  */

  switch (game->last_lines_cleared) {
    case 4:
      game->score += 800;
      game->l4_clear++;
      break;

    case 3:
      game->score += 500;
      game->l3_clear++;
      break;

    case 2:
      game->score += 300;
      game->l2_clear++;
      break;

    case 1:
      game->score += 100;
      game->l1_clear++;
      break;

    default:
      return 1;
  }

  return 0;
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

void tet_game_calculate(tet_Game game, tet_HashMap *visited,
    double *best_evaluation, tet_PiecePlacement *best_placement, tet_MoveList *best_moves, tet_MoveList *moves) {
  /*
     Evaluates all possible moves of current_piece for the given game
  */

  if (!tet_game_is_valid(&game) ||
      moves->size == MAX_MOVES ||
      tet_hashmap_get(visited, &game, NULL) == 0 ||
      (moves->size > 0 && moves->buffer[moves->size-1] == TET_MOVE_DROP)) {
    return;
  }

  tet_hashmap_set(visited, &game, 1);

  if (tet_game_can_place(&game)) {
    tet_Game copy = game;
    tet_game_place(&copy);
    double eval = tet_game_evaluate(&copy);
    if ((eval > *best_evaluation) ||
        (eval == *best_evaluation && moves->size < best_moves->size)) {
      *best_evaluation = eval;
      best_placement->position = game.position;
      best_placement->rotation = game.current_piece.rotation;
      *best_moves = *moves;
    }
    /*tet_debug_print_game(&copy, true);*/
  }

  tet_Move move_order[] = {
    TET_MOVE_DROP,
    TET_MOVE_DOWN,
    TET_MOVE_LEFT,
    TET_MOVE_RIGHT,
    TET_MOVE_CLOCKWISE,
    TET_MOVE_CLOCKWISE
  };

  for (int32_t i = 0; i < 6; i++) {
    if (tet_game_can_move(game, move_order[i])) {
      tet_Game copy = game;
      tet_game_move(&copy, move_order[i]);
      if (i != TET_MOVE_DOWN) {
        tet_game_move(&copy, TET_MOVE_DOWN);
      }
      moves->buffer[moves->size++] = move_order[i];
      tet_game_calculate(copy, visited, best_evaluation, best_placement, best_moves, moves);
      moves->size--;
    }
  }
}

double tet_game_evaluate(const tet_Game *game) {
  /*
     Evaluates the given game and returns its score based on all of the board parameters. Assumes
     that the board parameters of the given game are up to date
  */

  double evalutation = 0;

  int32_t max_height = game->heights[0];
  for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
    if (game->heights[col] > max_height) {
      max_height = game->heights[col];
    }
  }
  evalutation += (WEIGHT(game->chromosome.coeff_of_height) * max_height);
  evalutation += (WEIGHT(game->chromosome.coeff_of_line_clear[game->last_lines_cleared]));
  evalutation += (WEIGHT(game->chromosome.coeff_of_holes) * game->holes);
  evalutation += (WEIGHT(game->chromosome.coeff_of_bumpiness) * game->bumpiness);

  return evalutation;
}

void tet_game_play(tet_Game *game) {
  /*
     Play one best move of the given game
  */

  tet_HashMap map;
  if (tet_hashmap_init(&map) != 0) {
    perror("Failed to initialze hashmap.\n");
    exit(1);
  }

  tet_MoveList best_moves = { .buffer = {0}, .size = 0 };
  tet_MoveList moves = { .buffer = {0}, .size = 0 };
  tet_PiecePlacement best_placement;
  double best_evaluation = -DBL_MAX;

  tet_game_calculate(*game, &map, &best_evaluation, &best_placement, &best_moves, &moves);
  game->position = best_placement.position;
  game->current_piece.rotation = best_placement.rotation;

  tet_game_place(game);
  tet_game_end_turn(game);
  tet_hashmap_free(&map);
}
