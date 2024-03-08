#ifndef CLI_H
#define CLI_H

#include <ncurses.h>

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    cbreak();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

#define BOARDS_BEGIN 2
#define BOARD_N 20
#define BOARD_M 10
#define HUD_WIDTH 8

#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
void print_overlay(void);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

#endif