#include "cli.h"

void printPause(WINDOW *w) {
  box(w, 0, 0);
  mvwprintw(w, 10, 4, "PAUSE");
  wrefresh(w);
}

void printEnd(WINDOW *w) {
  box(w, 0, 0);
  refresh();

  mvwprintw(w, 5, 6, "GAME OVER");
  mvwprintw(w, 10, 5, "Press ENTER");
  mvwprintw(w, 11, 4, "to start again");

  wrefresh(w);
}

void printStart(WINDOW *w) {
  box(w, 0, 0);
  refresh();

  mvwprintw(w, 10, 5, "Press ENTER");
  mvwprintw(w, 11, 6, "to start");

  wrefresh(w);
}

void displayField(WINDOW *board, GameInfo_t *tetris) {
  box(board, 0, 0);
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (tetris->field[i][j]) {
        mvwaddch(board, i + 1, j + 1, BLOCK(tetris->field[i][j]));
      } else
        WPRINTW(board, i, j, " ");
    }
  }
  wrefresh(board);
}

void displayNextFigure(WINDOW *sidebar, GameInfo_t *tetris) {
  box(sidebar, 0, 0);
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      if (tetris->next[i][j]) {
        mvwaddch(sidebar, i + 12, j + 3,
                 ' ' | A_REVERSE | COLOR_PAIR(tetris->next[i][j]));
      } else
        mvwaddch(sidebar, i + 12, j + 3, ' ');
    }
  }
  wrefresh(sidebar);
}

void printStats(WINDOW *sidebar, GameInfo_t *tetris) {
  box(sidebar, 0, 0);
  mvwprintw(sidebar, 1, 2, "Score");
  mvwprintw(sidebar, 4, 2, "Record");
  mvwprintw(sidebar, 7, 2, "Level");
  mvwprintw(sidebar, 10, 2, "Next");
  mvwprintw(sidebar, 2, 4, "%d", tetris->score);
  mvwprintw(sidebar, 5, 4, "%d", tetris->high_score);
  mvwprintw(sidebar, 8, 4, "%d", tetris->level);
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