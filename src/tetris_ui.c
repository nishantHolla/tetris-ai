#include "tetris.h"

// UI Assets

const char *TET_UI_FONT_PATH = "assets/romulus.png";

// UI Fonts

Font fonts[MAX_FONTS] = {0};
const tet_FontSize TET_FONT_XL = { .size = 5.0f, .spacing = 8.0f };

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

void tet_ui_calculate() {
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
}

void tet_ui_draw_text() {
  /*
     Draws all required text onto the screen for the current frame
  */

  DrawTextEx(fonts[0], GAME_TITLE, tet_ui_title_text,
      fonts[0].baseSize * TET_FONT_XL.size, TET_FONT_XL.spacing, TET_UI_TEXT_COLOR);
}

void tet_ui_draw_board(const tet_Game *game) {
  /*
     Draws the current state of the board onto the screen
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
        cell_color = TET_UI_PIECE_COLOR[game->current_piece.index];
      }
      DrawRectangleRounded(cell, TET_UI_ROUNDEDNESS_SM, 0, cell_color);
    }
  }
}
