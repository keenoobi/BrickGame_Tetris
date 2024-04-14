#ifndef DATA_H
#define DATA_H

#define BOARDS_BEGIN 2
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define HUD_WIDTH 12
#define TETROMINO_SIZE 4
#define NUM_TETROMINOES 7
#define NUM_ORIENTATIONS 4
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

static const int kScoreForCompleteLiens[4] = {100, 300, 700, 1500};

static const int kGravityLevel[10] = {
    // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    50, 48, 46, 44, 42, 40, 38, 36, 34, 32};

static const tetris_location
    kTetrisFigure[NUM_TETROMINOES][NUM_ORIENTATIONS][TETROMINO_SIZE] = {
        // I
        {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
         {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
         {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
         {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
        // J
        {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
         {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
         {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
         {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
        // L
        {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
         {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
         {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
         {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
        // O
        {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
         {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
         {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
         {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
        // S
        {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
         {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
         {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
         {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
        // T
        {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
         {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
         {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
         {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
        // Z
        {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
         {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
         {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
         {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
};

#endif