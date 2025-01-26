#include "tetris.h"

// Debug Functions

void tet_debug_print_game(const tet_Game *game, bool be_verbose) {
  /*
     Prints the current state of the given game to stdout. Prints additional values if be_verbose
     flag is set
  */

  const uint16_t shape = TET_PIECE_SHAPE[game->current_piece.index][game->current_piece.rotation];
  const int8_t px = game->position.x;
  const int8_t py = game->position.y;

  printf(" ┏━━━━━━━━━━━━━━━━━━━━┓\n");
  for (int32_t row = GRID_ROW_COUNT - 1; row > -1; row--) {
    printf(" ┃");
    for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
      if (row >= py && row < py + 4 &&
          col >= px && col < px + 4 &&
          SHAPE_BIT(shape, row-py, col-px)) {
        printf("■ ");
      }
      else if (game->board[row][col] != GRID_EMPTY_CELL) {
        printf("☐ ");
      }
      else {
        printf("  ");
      }
    }
    printf("┃\n");
  }
  printf(" ┗━━━━━━━━━━━━━━━━━━━━┛\n");

  if (be_verbose) {
    printf("Position: %d %d\n", px, py);
    tet_debug_print_piece("Current Piece: ", &game->current_piece);
    tet_debug_print_piece("Next Piece: ", &game->next_piece);
    if (game->has_held_piece) {
      tet_debug_print_piece("Held Piece: ", &game->held_piece);
    }
    printf("Has held piece: %d\n", game->has_held_piece);
    printf("Has swapped piece: %d\n", game->has_swapped);
    printf("Score: %ld\n", game->score);
    uchar_t hash[SHA256_DIGEST_LENGTH];
    tet_hashmap_hash(game, hash);
    tet_debug_print_hash("Hash: ", hash);
    printf("Index: %d\n", tet_hashmap_index(hash));
  }
}

void tet_debug_print_piece(const char *pre_text, const tet_Piece *piece) {
  /*
     Prints piece name and rotation to stdout after printing the given pre_text
  */

  if (pre_text) {
    printf("%s", pre_text);
  }

  switch (piece->index) {
    case TET_PIECE_I: printf("TET_PIECE_I");
                      break;
    case TET_PIECE_L: printf("TET_PIECE_L");
                      break;
    case TET_PIECE_LR: printf("TET_PIECE_LR");
                      break;
    case TET_PIECE_O: printf("TET_PIECE_O");
                      break;
    case TET_PIECE_S: printf("TET_PIECE_S");
                      break;
    case TET_PIECE_SR: printf("TET_PIECE_SR");
                      break;
    case TET_PIECE_T: printf("TET_PIECE_T");
                      break;
    default:          printf("TET_PIECE_UNKNOWN");
                      break;
  }

  printf(" ");

  switch (piece->rotation) {
    case TET_ROTATION_NORTH: printf("TET_ROTATION_NORTH");
                             break;
    case TET_ROTATION_EAST:  printf("TET_ROTATION_EAST");
                             break;
    case TET_ROTATION_SOUTH: printf("TET_ROTATION_SOUTH");
                             break;
    case TET_ROTATION_WEST:  printf("TET_ROTATION_WEST");
                             break;
    default:                 printf("TET_ROTATION_UNKNOWN");
                             break;
  }

  printf("\n");
}

void tet_debug_print_hash(const char *pre_text, const uchar_t *hash) {
  /*
     Prints the given SHA256 hash to stdout after printing the given pre_text
  */

  printf("%s ", pre_text);
  for (int32_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
  printf("\n");
}
