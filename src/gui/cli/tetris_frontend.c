#include "tetris_frontend.h"

WINDOW *createNewWindow(WINDOW *w, int width, int x) {
  w = newwin(BOARD_HEIGHT + 2, width + 2, BOARDS_BEGIN, x);
  return w;
}

void printPause(WINDOW *w) {
  box(w, 0, 0);
  mvwprintw(w, 10, 8, "PAUSE");
  wrefresh(w);
}

void printEnd(WINDOW *w) {
  box(w, 0, 0);
  refresh();

  mvwprintw(w, 5, 13, "GAME OVER");
  mvwprintw(w, 10, 12, "Press ENTER");
  mvwprintw(w, 11, 10, "to start again");

  wrefresh(w);
}

void printStart(WINDOW *w) {
  box(w, 0, 0);
  refresh();

  mvwprintw(w, 10, 12, "Press ENTER");
  mvwprintw(w, 11, 13, "to start");

  wrefresh(w);
}

void displayField(WINDOW *board, GameInfo_t *tetris) {
  box(board, 0, 0);
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (tetris->field[i][j]) {
        ADD_BLOCK(board, i, j, tetris->field[i][j]);
      } else {
        ADD_EMPTY(board, i, j);
      }
    }
  }
  wrefresh(board);
}

void displayNextFigure(WINDOW *sidebar, GameInfo_t *tetris) {
  box(sidebar, 0, 0);
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      if (tetris->next[i][j]) {
        ADD_BLOCK(sidebar, i + 11, j + 1, tetris->next[i][j]);
      } else {
        ADD_EMPTY(sidebar, i + 11, j + 1);
      }
    }
  }
  wrefresh(sidebar);
}

void printStats(WINDOW *sidebar, GameInfo_t *tetris) {
  box(sidebar, 0, 0);
  mvwprintw(sidebar, 1, 4, "Score");
  mvwprintw(sidebar, 4, 4, "Record");
  mvwprintw(sidebar, 7, 4, "Level");
  mvwprintw(sidebar, 10, 4, "Next");
  mvwprintw(sidebar, 2, 6, "%d", tetris->score);
  mvwprintw(sidebar, 5, 6, "%d", tetris->high_score);
  mvwprintw(sidebar, 8, 6, "%d", tetris->level + 1);
  wrefresh(sidebar);
}

void initColors(void) {
  start_color();

  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_RED, COLOR_BLACK);
}