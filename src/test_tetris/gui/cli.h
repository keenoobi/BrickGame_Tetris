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
    initColors();            \
    timeout(time);           \
  }

#define BOARDS_BEGIN 2
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define HUD_WIDTH 12
#define TETROMINO_SIZE 4

#define ESC_KEY 27
#define ENTER_KEY 10

#define WPRINTW(window, y, x, ...) \
  mvwprintw((window), 1 + (y), 1 + (x), __VA_ARGS__)
#define ADD_BLOCK(win, row, col, x)                                         \
  mvwaddch((win), (row) + 1, (col)*2 + 1, ' ' | A_REVERSE | COLOR_PAIR(x)); \
  mvwaddch((win), (row) + 1, (col)*2 + 2, ' ' | A_REVERSE | COLOR_PAIR(x))

#define ADD_EMPTY(win, row, col)                \
  mvwaddch((win), (row) + 1, (col)*2 + 1, ' '); \
  mvwaddch((win), (row) + 1, (col)*2 + 2, ' ')

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
void printBoard(WINDOW *board, WINDOW *sidebar);
WINDOW *createNewWindow(WINDOW *w, int width, int x);
GameInfo_t *gameStateInit(int rows, int cols);
void freeGameInfo(GameInfo_t *tetris);
void displayField(WINDOW *board, GameInfo_t *tetris);
void initColors(void);
void displayNextFigure(WINDOW *sidebar, GameInfo_t *tetris);
void printStats(WINDOW *sidebar, GameInfo_t *tetris);
void printPause();

#endif