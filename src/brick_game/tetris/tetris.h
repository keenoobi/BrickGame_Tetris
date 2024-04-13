#ifndef TETRIS_H
#define TETRIS_H

#include "../../gui/cli/tetris_frontend.h"
#include "tetris_backend.h"

void gameLoop(windows *w, game *tetris, GameInfo_t *data);
#endif
