#include "cli.h"

void printBoard(WINDOW *board, WINDOW *sidebar) {
  box(board, 0, 0);
  box(sidebar, 0, 0);
  refresh();

  // mvwprintw(board, 1, 1, "@");
  mvwprintw(sidebar, 1, 2, "Score");
  mvwprintw(sidebar, 4, 2, "Record");
  mvwprintw(sidebar, 7, 2, "Level");
  mvwprintw(sidebar, 10, 2, "Next");
  // print_stats(sidebar);
  // wnoutrefresh(board);
  // wnoutrefresh(sidebar);
  wrefresh(board);
  wrefresh(sidebar);
}

void displayField(WINDOW *board, game *tetris) {
  for (int i = 0; i < tetris->rows; i++) {
    for (int j = 0; j < tetris->cols; j++) {
      if (getCell(tetris, i, j) == 0)
        WPRINTW(board, i, j, "#");
      else
        WPRINTW(board, i, j, " ");
    }
  }
  wrefresh(board);
}

GameInfo_t updateCurrentState() {
  GameInfo_t game;
  game.field
}

void print_stats(WINDOW *sidebar) {}

// void allocate_field(GameInfo_t *game) {}