#ifndef FSM_H
#define FSM_H

#include "../tetris.h"

typedef void (*action)(params_t *prms);

Signals_t get_signal(int user_input);
void sigact(Signals_t signal, windows *w, tetris_state *state, game *tetris,
            bool *game_over_flag);

void exitstate(params_t *prms);
void spawn(params_t *prms);
void shifting(params_t *prms);
void moveDown(params_t *prms);
void moveRight(params_t *prms);
void moveLeft(params_t *prms);
void rotate(params_t *prms);
void attaching(params_t *prms);
void pauseGame(params_t *prms);
void gameOver(params_t *prms);

#endif