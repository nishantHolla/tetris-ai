#ifndef TETRIS_H_
#define TETRIS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>
#include <string.h>
#include <float.h>
#include <getopt.h>
#include "raylib.h"

#define GRID_ROW_COUNT 20
#define GRID_COL_COUNT 10
#define NUMBER_OF_PIECES 7
#define NUMBER_OF_ROTATIONS 4
#define GRID_EMPTY_CELL 7
#define HASH_CPACITY 1000
#define MAX_MOVES 100
#define MAX_FONTS 8
#define GAME_TITLE "TETRIS"
#define NEXT_PIECE_TITLE "Next Piece"
#define HELD_PIECE_TITLE "Held Piece"
#define INSTRUCTIONS "Keybindings:\n\
    LEFT: Move piece left\n\
    RIGHT: Move piece right\n\
    SPACE: Drop piece\n\
    UP: Rotate piece anti clockwise\n\
    DOWN: Rotate piece clockwise\n\
    F: Swap with held piece\n\
    R: Restart game"
#define HELP_TEXT "Help:\n\n\
USAGE:\n\
======\n\
\n\
./tetris <options>\n\
    --no-ai -n: Play the game yourself without the AI\n\
    --train -t: Train the AI with custom train parameters\n\
    --help  -h: Print help message and exit\n\
\n\
TRAINING:\n\
=========\n\
\n\
To train the AI create a file called tetris-ai-train-params.txt in the directory of the executable\n\
with the following numbers in the first line separated by spaces\n\
    generation_count (positive integer): Number of generations to train\n\
    population_size (positive intenger): Size of the population to train\n\
    games_per_chromosome (positive integer): Number of games to play for each chromosome in the population\n\
    moves_per_game (positive integer): Maximum number of moves each game should be played for\n\
    elitism_rate (decimal between 0 and 1): Percentage of the population to be carried over to the next generation\n\
    mutation_rate (deciaml between 0 and 1): Percentage of population to be mutated in each generation\n\
\n\
After the training is complete, two new files called tetris-ai-train-<timestamp>.log and tetris-ai-params.txt will be\n\
created:\n\
    tetris-ai-train-<timestamp>.log file contains the log report of each generation of the training session.\n\
    tetris-ai-params.txt contains the parameters of the best chromosome after the end of the training session.\n\
Running the application again with the tetris-ai-params.txt file in the same directory will use those weights in the AI\n\
Default weights will be used if no such file is found in the current directory of the executable.\n\
\n\
Author:\n\
=======\n\
\n\
Nishant Holla (https://github.com/nishantHolla)\n\n"

/*#define POPULATION_SIZE 100*/
/*#define GENERATION_COUNT 10*/
/*#define GAMES_PER_CHROMOSOMES 10*/
/*#define MOVES_PER_GAME 1000*/
/*#define ELITISM_RATE 0.2*/
/*#define MUTATION_RATE 0.4*/
#define MAX_COEFFICIENT_MAGNITUDE 1000
#define MAX_MUTATION 40

#define SHAPE_BIT(s,r,c) (s & (1 << (((r)*4)+(c))))
#define ROTATE_CLOCKWISE(p) ((p.rotation + 1) % NUMBER_OF_ROTATIONS)
#define ROTATE_ANTI_CLOCKWISE(p) ((p.rotation + NUMBER_OF_ROTATIONS - 1) % NUMBER_OF_ROTATIONS)
#define WEIGHT(c) (c.magnitude * (c.is_positive ? 1 : -1))
#define RAYLIB_COLOR(R,G,B,A) { .r = R, .g = G, .b = B, .a = A}

// Definitions

typedef unsigned char uchar_t;

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

typedef enum tet_Move {
  TET_MOVE_DOWN,
  TET_MOVE_LEFT,
  TET_MOVE_RIGHT,
  TET_MOVE_CLOCKWISE,
  TET_MOVE_ANTI_CLOCKWISE,
  TET_MOVE_DROP,
  TET_MOVE_SWAP
} tet_Move;

typedef struct tet_TrainParameters {
  int32_t generation_count;
  int32_t population_size;
  int32_t games_per_chromosome;
  int32_t moves_per_game;
  double elitsm_rate;
  double mutation_rate;
} tet_TrainParameters;

typedef struct tet_Coefficient {
  double magnitude;
  int8_t is_positive;
} tet_Coefficient;

typedef struct tet_Chromosome {
  tet_Coefficient coeff_of_line_clear[5];
  tet_Coefficient coeff_of_bumpiness;
  tet_Coefficient coeff_of_holes;
  tet_Coefficient coeff_of_height;
  int64_t fitness;
} tet_Chromosome;

typedef struct tet_Position {
  int8_t x;
  int8_t y;
} tet_Position;

typedef struct tet_PiecePlacement {
  tet_Position position;
  tet_RotationIndex rotation;
} tet_PiecePlacement;

typedef struct tet_Piece {
  tet_PieceIndex index;
  tet_RotationIndex rotation;
} tet_Piece;

typedef struct tet_Game {
  bool is_over;
  int8_t board[GRID_ROW_COUNT][GRID_COL_COUNT];
  tet_Position position;
  tet_Piece current_piece;
  tet_Piece next_piece;
  tet_Piece held_piece;
  bool has_held_piece;
  bool has_swapped;
  int64_t score;
  int8_t heights[GRID_COL_COUNT];
  int32_t bumpiness;
  int32_t last_lines_cleared;
  int32_t holes;
  int64_t l4_clear;
  int64_t l3_clear;
  int64_t l2_clear;
  int64_t l1_clear;
  tet_Chromosome chromosome;
} tet_Game;

typedef struct tet_MoveList {
  tet_Move buffer[MAX_MOVES];
  size_t size;
} tet_MoveList;

typedef struct tet_HashBucket {
  uchar_t key[SHA256_DIGEST_LENGTH];
  double value;
  struct tet_HashBucket *next;
} tet_HashBucket;

typedef struct tet_HashMap {
  tet_HashBucket *buffer[HASH_CPACITY];
  size_t size;
} tet_HashMap;

typedef struct tet_FontSize {
  double size;
  double spacing;
} tet_FontSize;

// Game Values

extern bool AI_IS_PLAYING;
extern bool AI_IS_TRAINING;
extern const tet_Position START_POSITION;
extern const uint16_t TET_PIECE_SHAPE[NUMBER_OF_PIECES][NUMBER_OF_ROTATIONS];
extern const tet_Coefficient coeff_of_line_clear[5];
extern const tet_Coefficient coeff_of_bumpiness;
extern const tet_Coefficient coeff_of_holes;
extern const tet_Coefficient coeff_of_height;
extern const tet_Chromosome tet_default_chromosome;

// Game Functions

int8_t tet_game_init(tet_Game *game, const tet_Chromosome *chromosome);
bool tet_game_can_move(tet_Game game, const tet_Move move);
int8_t tet_game_move(tet_Game *game, const tet_Move move);
bool tet_game_can_place(const tet_Game *game);
int8_t tet_game_place(tet_Game *game);
int8_t tet_game_remove_filled_lines(tet_Game *game);
int8_t tet_game_end_turn(tet_Game *game);
bool tet_game_is_valid(const tet_Game *game);
int8_t tet_game_calculate_lines_removed(tet_Game *game);
int8_t tet_game_calculate_score(tet_Game *game);
int8_t tet_game_calculate_heights(tet_Game *game);
int8_t tet_game_calculate_holes(tet_Game *game);
int8_t tet_game_calculate_bumpiness(tet_Game *game);
void tet_game_calculate(tet_Game game, tet_HashMap *visited,
    double *best_evaluation, tet_PiecePlacement *best_placement, tet_MoveList *best_moves, tet_MoveList *moves);
double tet_game_evaluate(const tet_Game *game);
void tet_game_play(tet_Game *game);

// AI Functions

int8_t tet_ai_init_population(tet_Chromosome *population, const tet_TrainParameters *train_param);
int8_t tet_ai_init_chromosome(tet_Chromosome *chromosome);
int8_t tet_ai_train(tet_TrainParameters train_param);
int8_t tet_ai_calculate_fitness(tet_Chromosome *chromosome, const tet_TrainParameters *train_param);
int8_t tet_ai_select_and_crossover(tet_Chromosome *population, const tet_TrainParameters *train_param);
int8_t tet_ai_mutate(tet_Chromosome *population, const tet_TrainParameters *train_param);

// Hashmap Functions

int8_t tet_hashmap_hash(const tet_Game *game, uchar_t *hash);
int32_t tet_hashmap_index(const uchar_t *hash);
int8_t tet_hashmap_init(tet_HashMap *map);
int8_t tet_hashmap_get(tet_HashMap *map, const tet_Game *game, double *value);
int8_t tet_hashmap_set(tet_HashMap *map, const tet_Game *game, double value);
int8_t tet_hashmap_remove(tet_HashMap *map, const tet_Game *game);
void tet_hashmap_free(tet_HashMap *map);

// Util Functions

tet_Piece tet_generate_random_piece();
int32_t tet_generate_random_int(int32_t min, int32_t max);
double tet_generate_random_double(double min, double max);

// Debug Values

extern int32_t debug_print_count;
extern int32_t debug_4lc_count;
extern int32_t debug_3lc_count;
extern int32_t debug_2lc_count;
extern int32_t debug_1lc_count;

// UI Assests

extern const char * TET_UI_FONT_PATH;

// UI Fonts

extern Font fonts[MAX_FONTS];
extern const tet_FontSize TET_FONT_XL;
extern const tet_FontSize TET_FONT_LG;
extern const tet_FontSize TET_FONT_SM;

// UI Values

extern const double TET_UI_ROUNDEDNESS_SM;

// UI Colors

extern const Color TET_UI_WINDOW_BG_COLOR;
extern const Color TET_UI_TEXT_COLOR;
extern const Color TET_UI_EMPTY_CELL_COLOR;
extern const Color TET_UI_PIECE_COLOR[NUMBER_OF_PIECES];

// UI Elements

extern Vector2 tet_ui_title_text;
extern Vector2 tet_ui_tetris_board;
extern Vector2 tet_ui_board_cell;
extern Vector2 tet_ui_board_spacing;
extern Vector2 tet_ui_next_piece_text;
extern Vector2 tet_ui_held_piece_text;
extern Vector2 tet_ui_next_piece_board;
extern Vector2 tet_ui_held_piece_board;
extern Vector2 tet_ui_instructions_text;
extern Vector2 tet_ui_score_text;
extern Vector2 tet_ui_stats_text;

// UI Functions

void tet_ui_init_fonts();
void tet_ui_free_fonts();
void tet_ui_calculate(const tet_Game *game);
void tet_ui_draw_text(const tet_Game *game);
void tet_ui_draw_board(const tet_Game *game);
void tet_ui_draw_next_and_held_piece(const tet_Game *game);
void tet_ui_print_piece(const char *pre_text, const tet_Piece *piece, char *string);

// Debug Functions

void tet_debug_print_game(const tet_Game *game, bool be_verbose);
void tet_debug_print_piece(const char *pre_text, const tet_Piece *piece);
void tet_debug_print_hash(const char *pre_text, const uchar_t *hash);
void tet_debug_print_movelist(const char *pre_text, const tet_MoveList *list);

#endif // !TETRIS_H_
