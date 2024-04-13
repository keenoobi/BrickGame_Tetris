#ifndef DATA_H
#define DATA_H

#include "../tetris.h"

#define BOARDS_BEGIN 2
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define HUD_WIDTH 12
#define TETROMINO_SIZE 4

typedef enum {
  START,
  SPAWN,
  MOVING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE,
  PAUSE
} tetris_state;

typedef struct coord {
  int row;
  int col;
} tetris_location;

typedef struct tetromino {
  int type;
  int orient;
  tetris_location coordinates;
} tetris_block;

typedef struct tetris {
  int rows;
  int cols;
  int board[BOARD_HEIGHT][BOARD_WIDTH];
  int next_figure[TETROMINO_SIZE][TETROMINO_SIZE];
  int score;
  int record;
  int level;
  int tick_till_drop;
  tetris_block falling_tetromino;
  tetris_block next_tetromino;
} game;

typedef struct windows {
  WINDOW *board;
  WINDOW *sidebar;
  WINDOW *start;
  WINDOW *pause;
  WINDOW *end;
} windows;

typedef struct {
  game *stats;
  tetris_state *state;
  windows w;
  bool *game_over;
} params_t;

#endif