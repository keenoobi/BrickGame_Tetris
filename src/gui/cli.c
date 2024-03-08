#include "cli.h"

void print_overlay(void) {
  print_rectangle(0, BOARD_N + 1, 0, BOARD_M + 1);
  print_rectangle(0, BOARD_N + 1, BOARD_M + 2, BOARD_M + HUD_WIDTH + 2);

  //   print_rectangle(1, 4, BOARD_M + 3, BOARD_M + HUD_WIDTH + 1);

  MVPRINTW(2, BOARD_M + 4, "score");
  MVPRINTW(3, BOARD_M + 5, "123");
  MVPRINTW(5, BOARD_M + 4, "record");
  MVPRINTW(11, BOARD_M + 5, "next");
  MVPRINTW(8, BOARD_M + 5, "lvl");
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