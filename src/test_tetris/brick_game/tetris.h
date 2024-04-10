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
typedef struct coord {
  int row;
  int col;
} tetris_location;

typedef struct tetromino {
  int type;
  int orient;
  tetris_location coordinates;
} tetris_block;

// typedef struct {
//   int rows;
//   int cols;
//   char* board;
// } tetris_board;

typedef struct tetris {
  int rows;
  int cols;
  int **board;

  int **next_figure;

  int points;
  int level;

  int tick_till_drop;

  int points_remaining;
  GameInfo_t *next;

  tetris_block falling_tetromino;
  tetris_block next_tetromino;
} game;

typedef enum {
  START,
  SPAWN,
  MOVING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE,
  PAUSE
} tetris_state;

typedef struct game_params {
  game *stats;
  tetris_state *state;
  // board_t *map;
  // player_pos *frog_pos;
  // bool *break_flag;
} params_t;

typedef void (*action)(params_t *prms);

int getCell(game *tetris, int row, int column);
extern tetris_location TETRIS_FIGURE[7][4][4];
extern int GRAVITY_LEVEL[19 + 1];

bool tetrominoFits(game *tetris, tetris_block block);
void placeTetromino(game *tetris, tetris_block piece);
void removeTetromino(game *tetris, tetris_block piece);
void newFallingFigure(game *tetris);
void setCell(game *tetris, int row, int column, int value);
int getCell(game *tetris, int row, int column);
void attaching(params_t *prms);
void placeNextTetromino(game *tetris, tetris_block piece);
void removeNextTetromino(game *tetris, tetris_block piece);
#endif
