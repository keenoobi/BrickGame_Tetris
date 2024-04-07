#include "cli.h"

void print_overlay() {
  print_rectangle(0, BOARD_HEIGHT + 1, 0, BOARD_WIDTH + 1);
  print_rectangle(0, BOARD_HEIGHT + 1, BOARD_WIDTH + 2,
                  BOARD_WIDTH + HUD_WIDTH + 2);

  // print_rectangle(1, 4, BOARD_WIDTH + 3, BOARD_WIDTH + HUD_WIDTH + 1);

  MVPRINTW(2, BOARD_WIDTH + 4, "score");
  // MVPRINTW(3, BOARD_WIDTH + 5, "123");
  MVPRINTW(5, BOARD_WIDTH + 4, "record");
  MVPRINTW(11, BOARD_WIDTH + 4, "next");
  MVPRINTW(8, BOARD_WIDTH + 4, "level");
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_figure(test *a) {
  // PRINT_FROG(a->pos_x, a->pos_y);
  // int i = a->pos_x;
  // int j = a->pos_y;
  int square[4][4] = {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (square[i][j] == 1) {
        PRINT_FROG(a->pos_x + i, a->pos_y + j);  // Умножаем j на 2 для учета
                                                 // ширины символов в терминале
      }
    }
  }
}

void clear_figure(test *a) {
  int square[4][4] = {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (square[i][j]) {
        CLEAR_BACKPOS(a->pos_y + j,
                      a->pos_x + i);  // Умножаем j на 2 для учета
                                      // ширины символов в терминале
      }
    }
  }
}
