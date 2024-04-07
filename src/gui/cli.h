#ifndef CLI_H
#define CLI_H

#include <ncurses.h>

#define WIN_INIT(time)       \
  {                          \
    initscr();               \
    noecho();                \
    cbreak();                \
    nodelay(stdscr, true);   \
    notimeout(stdscr, TRUE); \
    curs_set(0);             \
    keypad(stdscr, TRUE);    \
    timeout(time);           \
  }

#define BOARDS_BEGIN 2
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define HUD_WIDTH 8

typedef struct {
  int pos_x;
  int pos_y;
} test;

#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define PRINT_FROG(x, y) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), "@")
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')
void print_overlay();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_figure(test *a);
void clear_figure(test *a);
#endif