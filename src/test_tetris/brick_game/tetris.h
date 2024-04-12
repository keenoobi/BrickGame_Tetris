#ifndef TETRIS_H
#define TETRIS_H

#include "../gui/cli.h"

#define POINTS_PER_LEVEL 600
#define NUM_TETROMINOES 7
#define EMPTY_CELL 0
#define CONVERT_TO_CELL(x) (x + 1)
#define RECORDS_FILE "record.save"

#define TRANSFER_DATA(ROWS, COLS, DEST, SRC) \
  {                                          \
    for (int _i = 0; _i < ROWS; _i++) {      \
      for (int _j = 0; _j < COLS; _j++) {    \
        DEST[_i][_j] = SRC[_i][_j];          \
      }                                      \
    }                                        \
  }

static const int kScoreForCompleteLiens[4] = {100, 300, 700, 1500};

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
  int board[BOARD_HEIGHT][BOARD_WIDTH];
  int next_figure[TETROMINO_SIZE][TETROMINO_SIZE];

  int score;
  int record;
  int level;

  int tick_till_drop;

  int points_remaining;

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
  // board_t *map;
  // player_pos *frog_pos;
  // bool *break_flag;
} params_t;

typedef void (*action)(params_t *prms);

int getCell(game *tetris, int row, int column);
extern const tetris_location TETRIS_FIGURE[7][4][4];
extern int GRAVITY_LEVEL[19 + 1];

WINDOW *createNewWindow(WINDOW *w, int width, int x);
game *gameInit(int rows, int cols);

bool tetrominoFits(game *tetris, tetris_block block);
void placeTetromino(game *tetris, tetris_block piece);
void removeTetromino(game *tetris, tetris_block piece);
void newFallingFigure(game *tetris);
void setCell(game *tetris, int row, int column, int value);
int getCell(game *tetris, int row, int column);
void attaching(params_t *prms);
void placeNextTetromino(game *tetris, tetris_block piece);
void removeNextTetromino(game *tetris, tetris_block piece);
void dataProcessing(int **data, int rows, int cols, int value);
#endif
