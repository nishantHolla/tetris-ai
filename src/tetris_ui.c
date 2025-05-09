#include "tetris.h"

// UI Assets

const char *TET_UI_FONT_PATH = "assets/romulus.png";

// UI Fonts

Font fonts[MAX_FONTS] = {0};
const tet_FontSize TET_FONT_XL = { .size = 5.0f, .spacing = 8.0f };
const tet_FontSize TET_FONT_LG = { .size = 3.0f, .spacing = 5.0f };
const tet_FontSize TET_FONT_SM = { .size = 2.5f, .spacing = 3.0f };

// UI Values

const double TET_UI_ROUNDEDNESS_SM = 0.05f;

// UI Colors

const Color TET_UI_WINDOW_BG_COLOR = RAYLIB_COLOR(9, 9, 11, 255);
const Color TET_UI_TEXT_COLOR = RAYLIB_COLOR(212, 212, 216, 255);
const Color TET_UI_EMPTY_CELL_COLOR = RAYLIB_COLOR(63, 63, 70, 255);
const Color TET_UI_PIECE_COLOR[NUMBER_OF_PIECES] = {
  RAYLIB_COLOR(34, 211, 238, 255), // TET_PIECE_I
  RAYLIB_COLOR(245, 158, 11, 255), // TET_PIECE_L
  RAYLIB_COLOR(99, 102, 241, 255), // TET_PIECE_LR
  RAYLIB_COLOR(250, 204, 21, 255), // TET_PIECE_O
  RAYLIB_COLOR(163, 230, 53, 255), // TET_PIECE_S
  RAYLIB_COLOR(239, 68, 68, 255), // TET_PIECE_SR
  RAYLIB_COLOR(168, 85, 247, 255), // TET_PIECE_T
};

// UI Elements

Vector2 tet_ui_title_text;
Vector2 tet_ui_tetris_board;
Vector2 tet_ui_board_cell = {.x = 44, .y = 44};
Vector2 tet_ui_board_spacing = { .x = 5, .y = 5};
Vector2 tet_ui_next_piece_text;
Vector2 tet_ui_held_piece_text;
Vector2 tet_ui_next_piece_board;
Vector2 tet_ui_held_piece_board;
Vector2 tet_ui_instructions_text;
Vector2 tet_ui_score_text;
Vector2 tet_ui_stats_text;

// UI Functions

void tet_ui_init_fonts() {
  /*
     Initializes fonts for the ui in the global fonts buffer
  */

  fonts[0] = LoadFont(TET_UI_FONT_PATH);
}

void tet_ui_free_fonts() {
  /*
     Fress fonts present in the global fonts buffer
  */

  for (int i =0; i < MAX_FONTS; i++) {
    UnloadFont(fonts[i]);
  }
}

void tet_ui_calculate(const tet_Game *game) {
  /*
     Calculates the posisition and size of all UI elements for the current frame
  */

  Vector2 title_measure = MeasureTextEx(fonts[0], GAME_TITLE,
      fonts[0].baseSize * TET_FONT_XL.size, TET_FONT_XL.spacing);
  tet_ui_title_text = (Vector2) {
    .x = GetScreenWidth() / 2 - title_measure.x / 2,
    .y = 10
  };

  tet_ui_tetris_board = (Vector2) {
    .x = GetScreenWidth() / 2 - (((tet_ui_board_cell.x + tet_ui_board_spacing.y)*GRID_COL_COUNT)) / 2,
    .y = tet_ui_title_text.y + title_measure.y + 20
  };

  Vector2 next_piece_measure = MeasureTextEx(fonts[0], NEXT_PIECE_TITLE,
      fonts[0].baseSize * TET_FONT_LG.size, TET_FONT_LG.spacing);
  tet_ui_next_piece_text = (Vector2) {
    .x = tet_ui_tetris_board.x - next_piece_measure.x - 20,
    .y = GetScreenHeight() - (tet_ui_board_cell.y + tet_ui_board_spacing.y) * 4 - next_piece_measure.y - 40
  };

  Vector2 held_piece_measure = MeasureTextEx(fonts[0], HELD_PIECE_TITLE,
      fonts[0].baseSize * TET_FONT_LG.size, TET_FONT_LG.spacing);
  tet_ui_held_piece_text = (Vector2) {
    .x = tet_ui_tetris_board.x - held_piece_measure.x - 20,
    .y = tet_ui_next_piece_text.y - (tet_ui_board_cell.y + tet_ui_board_spacing.y) * 4 - next_piece_measure.y - 40
  };

  tet_ui_next_piece_board = (Vector2) {
    .x = tet_ui_next_piece_text.x,
    .y = tet_ui_next_piece_text.y + next_piece_measure.y + 10
  };

  tet_ui_held_piece_board = (Vector2) {
    .x = tet_ui_held_piece_text.x,
    .y = tet_ui_held_piece_text.y + held_piece_measure.y + 10
  };

  Vector2 instruction_measure = MeasureTextEx(fonts[0], INSTRUCTIONS,
      fonts[0].baseSize * TET_FONT_SM.size, TET_FONT_SM.spacing);
  tet_ui_instructions_text = (Vector2) {
    .x = tet_ui_tetris_board.x + ((tet_ui_board_cell.x + tet_ui_board_spacing.y) * GRID_COL_COUNT) + 10,
    .y = GetScreenHeight() - instruction_measure.y - 10
  };

  char score_text[100];
  snprintf(score_text, 100, "Score: %ld", game->score);
  Vector2 score_measure = MeasureTextEx(fonts[0], score_text,
      fonts[0].baseSize * TET_FONT_LG.size, TET_FONT_LG.spacing);
  tet_ui_score_text = (Vector2) {
    .x = tet_ui_tetris_board.x - score_measure.x - 10,
    .y = tet_ui_held_piece_text.y - score_measure.y - 40
  };

  tet_ui_stats_text = (Vector2) {
    .x = tet_ui_tetris_board.x + ((tet_ui_board_cell.x + tet_ui_board_spacing.y) * GRID_COL_COUNT) + 10,
    .y = tet_ui_tetris_board.y
  };
}

void tet_ui_draw_text(const tet_Game *game) {
  /*
     Draws all required text onto the screen for the current frame
  */

  DrawTextEx(fonts[0], GAME_TITLE, tet_ui_title_text,
      fonts[0].baseSize * TET_FONT_XL.size, TET_FONT_XL.spacing, TET_UI_TEXT_COLOR);
  DrawTextEx(fonts[0], NEXT_PIECE_TITLE, tet_ui_next_piece_text,
      fonts[0].baseSize * TET_FONT_LG.size, TET_FONT_LG.spacing, TET_UI_TEXT_COLOR);
  DrawTextEx(fonts[0], HELD_PIECE_TITLE, tet_ui_held_piece_text,
      fonts[0].baseSize * TET_FONT_LG.size, TET_FONT_LG.spacing, TET_UI_TEXT_COLOR);
  DrawTextEx(fonts[0], AI_IS_PLAYING ? "AI is Playing" : INSTRUCTIONS, tet_ui_instructions_text,
      fonts[0].baseSize * TET_FONT_SM.size, TET_FONT_SM.spacing, TET_UI_TEXT_COLOR);
  char score_text[100];
  snprintf(score_text, 100, "Score: %ld", game->score);
  DrawTextEx(fonts[0], score_text, tet_ui_score_text,
      fonts[0].baseSize * TET_FONT_LG.size, TET_FONT_LG.spacing, TET_UI_TEXT_COLOR);

  char stats[500];
  tet_ui_print_piece("Current piece:\n", &game->current_piece, stats);
  tet_ui_print_piece("\nNext piece:\n", &game->next_piece, stats + strlen(stats));
  if (game->has_held_piece) {
    tet_ui_print_piece("\nHeld piece:\n", &game->held_piece, stats + strlen(stats));
  }
  sprintf(stats + strlen(stats), "\nBoard hash:\n");
  uchar_t hash[SHA256_DIGEST_LENGTH];
  tet_hashmap_hash(game, hash);
  for (int32_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(stats + strlen(stats), "%02x", hash[i]);
  }
  sprintf(stats + strlen(stats), "\n");
  sprintf(stats + strlen(stats), "\nHeights:\n");
  for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
    sprintf(stats + strlen(stats), "%d ", game->heights[col]);
  }
  sprintf(stats + strlen(stats), "\n");
  sprintf(stats + strlen(stats), "\nBumpiness:\n%d\n", game->bumpiness);
  sprintf(stats + strlen(stats), "\nHoles:\n%d\n", game->holes);
  sprintf(stats + strlen(stats), "\nTotal lines cleared:\n%ld\n",
      game->l1_clear + game->l2_clear + game->l3_clear + game->l4_clear);


  DrawTextEx(fonts[0], stats, tet_ui_stats_text,
      fonts[0].baseSize * TET_FONT_SM.size, TET_FONT_SM.spacing, TET_UI_TEXT_COLOR);
}

void tet_ui_draw_board(const tet_Game *game) {
  /*
     Draws the current state of the board onto the screen for the current frame
  */

  const uint16_t shape = TET_PIECE_SHAPE[game->current_piece.index][game->current_piece.rotation];
  const int8_t px = game->position.x;
  const int8_t py = game->position.y;

  for (int32_t row = GRID_ROW_COUNT-1; row > -1; row--) {
    for (int32_t col = 0; col < GRID_COL_COUNT; col++) {
      Rectangle cell = (Rectangle) {
        .x = tet_ui_tetris_board.x + ((tet_ui_board_cell.x + tet_ui_board_spacing.x) * col),
        .y = tet_ui_tetris_board.y + ((tet_ui_board_cell.y + tet_ui_board_spacing.y) * (GRID_ROW_COUNT - 1 - row)),
        .width = tet_ui_board_cell.x,
        .height = tet_ui_board_cell.y
      };
      Color cell_color = TET_UI_EMPTY_CELL_COLOR;
      if (row >= py && row < py + 4 &&
          col >= px && col < px + 4 &&
          SHAPE_BIT(shape, row-py, col-px)) {
        cell_color = TET_UI_PIECE_COLOR[game->current_piece.index];
      }
      else if (game->board[row][col] != GRID_EMPTY_CELL) {
        cell_color = TET_UI_PIECE_COLOR[game->board[row][col]];
      }
      DrawRectangleRounded(cell, TET_UI_ROUNDEDNESS_SM, 0, cell_color);
    }
  }
}

void tet_ui_draw_next_and_held_piece(const tet_Game *game) {
  /*
     Draws the next piece and held piece onto the screen for the current frame
  */

  const uint16_t next_shape = TET_PIECE_SHAPE[game->next_piece.index][game->next_piece.rotation];

  for (int32_t row = 3; row > -1; row--) {
    for (int32_t col = 0; col < 4; col++) {
      if (SHAPE_BIT(next_shape, row, col)) {
        Rectangle cell = (Rectangle) {
          .x = tet_ui_next_piece_board.x + ((tet_ui_board_cell.x + tet_ui_board_spacing.x) * col),
          .y = tet_ui_next_piece_board.y + ((tet_ui_board_cell.y + tet_ui_board_spacing.y) * (4-row-1)),
          .width = tet_ui_board_cell.x,
          .height = tet_ui_board_cell.y
        };
        DrawRectangleRounded(cell, TET_UI_ROUNDEDNESS_SM, 0, TET_UI_PIECE_COLOR[game->next_piece.index]);
      }
    }
  }

  if (game->has_held_piece) {
    const uint16_t held_shape = TET_PIECE_SHAPE[game->held_piece.index][game->held_piece.rotation];

    for (int32_t row = 3; row > -1; row--) {
      for (int32_t col = 0; col < 4; col++) {
        if (SHAPE_BIT(held_shape, row, col)) {
          Rectangle cell = (Rectangle) {
            .x = tet_ui_held_piece_board.x + ((tet_ui_board_cell.x + tet_ui_board_spacing.x) * col),
              .y = tet_ui_held_piece_board.y + ((tet_ui_board_cell.y + tet_ui_board_spacing.y) * (4-row-1)),
              .width = tet_ui_board_cell.x,
              .height = tet_ui_board_cell.y
          };
          DrawRectangleRounded(cell, TET_UI_ROUNDEDNESS_SM, 0, TET_UI_PIECE_COLOR[game->held_piece.index]);
        }
      }
    }
  }
}

void tet_ui_print_piece(const char *pre_text, const tet_Piece *piece, char *string) {
  /*
     Prints the name and rotation of given piece to string after the pre_text
  */

  string += sprintf(string, "%s", pre_text);

  switch (piece->index) {
    case TET_PIECE_I: string += sprintf(string, "TET_PIECE_I");
                      break;
    case TET_PIECE_L: string += sprintf(string, "TET_PIECE_L");
                      break;
    case TET_PIECE_LR: string += sprintf(string, "TET_PIECE_LR");
                      break;
    case TET_PIECE_O: string += sprintf(string, "TET_PIECE_O");
                      break;
    case TET_PIECE_S: string += sprintf(string, "TET_PIECE_S");
                      break;
    case TET_PIECE_SR: string += sprintf(string, "TET_PIECE_SR");
                      break;
    case TET_PIECE_T: string += sprintf(string, "TET_PIECE_T");
                      break;
    default:          string += sprintf(string, "TET_PIECE_UNKNOWN");
                      break;
  }

  string += sprintf(string, " ");

  switch (piece->rotation) {
    case TET_ROTATION_NORTH: string += sprintf(string, "TET_ROTATION_NORTH");
                             break;
    case TET_ROTATION_EAST:  string += sprintf(string, "TET_ROTATION_EAST");
                             break;
    case TET_ROTATION_SOUTH: string += sprintf(string, "TET_ROTATION_SOUTH");
                             break;
    case TET_ROTATION_WEST:  string += sprintf(string, "TET_ROTATION_WEST");
                             break;
    default:                 string += sprintf(string, "TET_ROTATION_UNKNOWN");
                             break;
  }

  sprintf(string, "\n");
}
