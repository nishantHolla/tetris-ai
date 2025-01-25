#ifndef TETRIS_H_
#define TETRIS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define GRID_ROW_COUNT 20
#define GRID_COL_COUNT 10
#define NUMBER_OF_PIECES 7
#define NUMBER_OF_ROTATIONS 4
#define GRID_EMPTY_CELL 7

#define SHAPE_BIT(s,r,c) (s & (1 << (((r)*4)+(c))))

// Definitions

typedef enum tet_PieceIndex {
  TET_PIECE_I,
  TET_PIECE_L,
  TET_PIECE_LR,
  TET_PIECE_O,
  TET_PIECE_S,
  TET_PIECE_SR,
  TET_PIECE_T
} tet_PieceIndex;

typedef enum tet_RotationIndex {
  TET_ROTATION_NORTH,
  TET_ROTATION_EAST,
  TET_ROTATION_SOUTH,
  TET_ROTATION_WEST
} tet_RotationIndex;

typedef struct tet_Position {
  int8_t x;
  int8_t y;
} tet_Position;

typedef struct tet_Piece {
  tet_PieceIndex index;
  tet_RotationIndex rotation;
} tet_Piece;

typedef struct tet_Game {
  int8_t board[GRID_ROW_COUNT][GRID_COL_COUNT];
  tet_Position position;
  tet_Piece current_piece;
  tet_Piece next_piece;
  tet_Piece held_piece;
  bool has_held_piece;
  bool has_swapped;
  int64_t score;
} tet_Game;

// Game Values

extern const tet_Position START_POSITION;
extern const uint16_t TET_PIECE_SHAPE[NUMBER_OF_PIECES][NUMBER_OF_ROTATIONS];

// Game Functions

int8_t tet_game_init(tet_Game *game);

// Util Functions

tet_Piece tet_generate_random_piece();
int32_t tet_generate_random_int(int32_t min, int32_t max);

// Debug functions

void tet_debug_print_game(const tet_Game *game, bool be_verbose);
void tet_debug_print_piece(const char *pre_text, const tet_Piece *piece);

#endif // !TETRIS_H_
