#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H

#include <ncurses.h>

#include "../../brick_game/tetris/inc/data.h"
#include "../../brick_game/tetris/inc/defines.h"

#define WIN_INIT(time)       \
  {                          \
    initscr();               \
    noecho();                \
    cbreak();                \
    nodelay(stdscr, true);   \
    notimeout(stdscr, TRUE); \
    curs_set(0);             \
    keypad(stdscr, TRUE);    \
    initColors();            \
    timeout(time);           \
  }

typedef struct {
  int field[BOARD_HEIGHT][BOARD_WIDTH];
  int next[TETROMINO_SIZE][TETROMINO_SIZE];
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  None = 0,
  Down,
  Right,
  Left,
  Rotate,
  ESC,
  Enter,
  Pause
} Signals_t;

void printStart(WINDOW *w);
void printEnd(WINDOW *w);
// void printBoard(WINDOW *board, WINDOW *sidebar);
// GameInfo_t *gameStateInit(int rows, int cols);
// void freeGameInfo(GameInfo_t *tetris);
void displayField(WINDOW *board, GameInfo_t *tetris);
void initColors(void);
void displayNextFigure(WINDOW *sidebar, GameInfo_t *tetris);
void printStats(WINDOW *sidebar, GameInfo_t *tetris);
void printPause(WINDOW *w);
WINDOW *createNewWindow(WINDOW *w, int width, int x);

#endif