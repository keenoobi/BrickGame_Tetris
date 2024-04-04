#include "cli.h"

void printBoard(WINDOW *board, WINDOW *sidebar) {
  box(board, 0, 0);
  box(sidebar, 0, 0);
  refresh();

  // mvwprintw(board, 1, 1, "@");
  // mvwprintw(sidebar, 1, 2, "Score");
  // mvwprintw(sidebar, 4, 2, "Record");
  // mvwprintw(sidebar, 7, 2, "Level");
  // mvwprintw(sidebar, 10, 2, "Next");
  // print_stats(sidebar);
  // wnoutrefresh(board);
  // wnoutrefresh(sidebar);
  wrefresh(board);
  wrefresh(sidebar);
}

void freeFieldAndFigure(GameInfo_t *tetris) {
  if (tetris->field) free(tetris->field);
  if (tetris->next) free(tetris->next);
}

void freeGameInfo(GameInfo_t *tetris) {
  if (tetris) {
    freeFieldAndFigure(tetris);
    free(tetris);
  }
}

int **allocateMemory(int height, int width) {
  int **field = (int **)calloc(height, sizeof(int *) + width * sizeof(int));
  if (field != NULL) {
    field[0] = (int *)(field + height);
    for (int i = 1; i < height; i++) {
      field[i] = field[0] + i * width;
    }
  }
  return field;
}

GameInfo_t *gameStateInit(int rows, int cols) {
  GameInfo_t *tetris = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  tetris->field = allocateMemory(BOARD_HEIGHT + 2, BOARD_WIDTH);
  tetris->next = allocateMemory(TETROMINO_SIZE, TETROMINO_SIZE);
  tetris->score = 0;
  tetris->high_score = 0;
  tetris->level = 0;
  tetris->speed = 0;
  tetris->pause = 0;

  return tetris;
}

void displayField(WINDOW *board, GameInfo_t *tetris) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (tetris->field[i][j]) {
        WPRINTW(board, i, j, "1");
      } else
        WPRINTW(board, i, j, " ");
    }
  }
  wrefresh(board);
}
// GameInfo_t updateCurrentState() {
//   GameInfo_t game;
//   game.field
// }

void print_stats(WINDOW *sidebar) {}

// void allocate_field(GameInfo_t *game) {}