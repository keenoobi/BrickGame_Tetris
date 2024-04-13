#ifndef DEFINES_H
#define DEFINES_H

#define POINTS_PER_LEVEL 600
#define NUM_TETROMINOES 7
#define NUM_ORIENTATIONS 4
#define TETROMINO_SIZE 4
#define EMPTY_CELL 0
#define CONVERT_TO_CELL(x) (x + 1)
#define RECORDS_FILE "record.save"

#define BOARDS_BEGIN 2
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define HUD_WIDTH 12

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

#define TRANSFER_DATA(ROWS, COLS, DEST, SRC) \
  {                                          \
    for (int _i = 0; _i < ROWS; _i++) {      \
      for (int _j = 0; _j < COLS; _j++) {    \
        DEST[_i][_j] = SRC[_i][_j];          \
      }                                      \
    }                                        \
  }

static const int kScoreForCompleteLiens[4] = {100, 300, 700, 1500};

static const int GRAVITY_LEVEL[19 + 1] = {
    // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    50, 48, 46, 44, 42, 40, 38, 36, 34, 32,
    // 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    30, 28, 26, 24, 22, 20, 16, 12, 8, 4};

static const tetris_location
    TETRIS_FIGURE[NUM_TETROMINOES][NUM_ORIENTATIONS][TETROMINO_SIZE] = {
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