#ifndef TETRIS_H
#define TETRIS_H

#include "../gui/cli.h"

typedef struct {
  int rows, cols;
  int **board;
} game;

typedef struct {
  int pos_x;
  int pos_y;
} test;

// void game_init(GameInfo_t *front, )

#endif
