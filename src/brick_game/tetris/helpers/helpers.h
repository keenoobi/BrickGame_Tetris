#ifndef HELPERS_H
#define HELPERS_H

#include "../tetris.h"

void checkRotationPossibility(params_t *prms, int col, int ori);
bool lineFull(game *tetris, int row);
void shiftDown(game *tetris, int row);
void resetData(params_t *prms);
// mutators
int getCell(game *tetris, int row, int column);
void setCell(game *tetris, int row, int column, int value);
//
bool checkBounds(game *tetris, int row, int column);
void randomSortArray(int *arr);
void gameInit(game *tetris, int rows, int cols);
void graphicProcessing(WINDOW *board, WINDOW *sidebar, GameInfo_t *data,
                       tetris_state *state);

#endif