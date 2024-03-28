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
#define BOARD_N 20
#define BOARD_M 10
#define HUD_WIDTH 8

#define WPRINTW(y, x, window, ...) \
  mvwprintw((window), BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void print_board();

#endif