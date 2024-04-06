#ifndef CLI_H
#define CLI_H

#include <ncurses.h>
#include <stdlib.h>

// #include "../brick_game/tetris.h"

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
#define TETROMINO_SIZE 4

#define ESC_KEY 27

#define WPRINTW(window, y, x, ...) \
  mvwprintw((window), 1 + (y), 1 + (x), __VA_ARGS__)

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);
// GameInfo_t updateCurrentState();

void printBoard(WINDOW *board, WINDOW *sidebar);
GameInfo_t *gameStateInit(int rows, int cols);
void freeGameInfo(GameInfo_t *tetris);
void displayField(WINDOW *board, GameInfo_t *tetris);

// void displayBoard(WINDOW *board, game *tetris);
// void print_stats(WINDOW *sidebar);

#endif