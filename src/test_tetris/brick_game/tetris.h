#ifndef TETRIS_H
#define TETRIS_H

#include "../gui/cli.h"

#define POINTS_PER_LEVEL 600
#define NUM_TETROMINOES 7
#define CONVERT_TO_CELL(x) (x + 1)

// typedef struct {
//   int rows, cols;
//   int **board;
// } game;

// typedef struct {
//   int pos_x;
//   int pos_y;
// } test;

// void game_init(GameInfo_t *front, )
typedef struct {
  int row;
  int col;
} tetris_location;

typedef struct {
  int type;
  int orient;
  tetris_location coordinates;
} tetris_block;

// typedef struct {
//   int rows;
//   int cols;
//   char* board;
// } tetris_board;

typedef struct {
  int rows;
  int cols;
  int **board;

  int points;
  int level;

  int tick_till_drop;

  int points_remaining;

  tetris_block falling_tetromino;
  tetris_block next_tetromino;
} game;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  STOP,
  GAMEOVER,
  EXIT_STATE
} tetris_state;

int getCell(game *tetris, int row, int column);
extern tetris_location TETRIS_FIGURE[7][4][4];
extern int GRAVITY_LEVEL[19 + 1];

#endif
