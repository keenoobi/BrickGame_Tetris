#include "tetris.h"

#include <stdlib.h>
#include <time.h>

void exitstate(params_t *prms) { *prms->state = EXIT_STATE; }

void spawn(params_t *prms) {
  newFallingFigure(prms->stats);
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino))
    *prms->state = GAMEOVER;
  else {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
    *prms->state = MOVING;
  }
}

void moveDown(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.coordinates.row++;
  if (tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  } else {
    prms->stats->falling_tetromino.coordinates.row--;
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
    *prms->state = ATTACHING;
  }
}

void moveRight(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.coordinates.col++;
  if (tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);

  } else {
    prms->stats->falling_tetromino.coordinates.col--;
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  }
}

void moveLeft(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.coordinates.col--;
  if (tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  } else {
    prms->stats->falling_tetromino.coordinates.col++;
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  }
}

void pauseGame(params_t *prms) {
  if (*prms->state != PAUSE) {
    *prms->state = PAUSE;
    printPause(prms->w.pause);
  } else {
    wclear(prms->w.pause);
    wrefresh(prms->w.pause);
    *prms->state = MOVING;
  }
  delwin(prms->w.pause);
}

void checkRotationPossibility(params_t *prms, int col, int ori) {
  // checking right side
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col >= 7)
    prms->stats->falling_tetromino.coordinates.col--;
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col >= 7)
    prms->stats->falling_tetromino.coordinates.col--;
  // checking left side
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col <= 2)
    prms->stats->falling_tetromino.coordinates.col++;
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col <= 2)
    prms->stats->falling_tetromino.coordinates.col++;
  // default check
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    prms->stats->falling_tetromino.coordinates.col = col;
    prms->stats->falling_tetromino.orient = ori;
    if (prms->stats->falling_tetromino.orient < 0)
      prms->stats->falling_tetromino.orient = 3;
  }
}

void rotate(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  int tmp_col = prms->stats->falling_tetromino.coordinates.col;
  int tmp_ori = prms->stats->falling_tetromino.orient;

  if (prms->stats->falling_tetromino.type != 3 &&
      prms->stats->falling_tetromino.orient++ >= 3)
    prms->stats->falling_tetromino.orient = 0;

  checkRotationPossibility(prms, tmp_col, tmp_ori);
  placeTetromino(prms->stats, prms->stats->falling_tetromino);
}

void shifting(params_t *prms) {
  if (prms->stats->tick_till_drop-- <= 0) {
    prms->stats->tick_till_drop = GRAVITY_LEVEL[prms->stats->level];
    moveDown(prms);
  }
}

bool lineFull(game *tetris, int row) {
  bool full = true;
  for (int j = 0; j < tetris->cols && full; j++) {
    if (getCell(tetris, row, j) == 0) full = false;
  }
  return full;
}

void shiftDown(game *tetris, int row) {
  for (int i = row - 1; i >= 0; i--) {
    for (int j = 0; j < tetris->cols; j++) {
      setCell(tetris, i + 1, j, getCell(tetris, i, j));
    }
  }
}

bool loadHighestScore(int *record, const char *filename) {
  bool state = true;
  FILE *load = fopen(filename, "rb");
  if (!load) return state = false;

  size_t count = fread(record, sizeof(int), 1, load);
  fclose(load);

  if (!count) state = false;
  return state;
}

bool saveHighestScore(int *record, const char *filename) {
  bool state = true;
  FILE *save = fopen(filename, "wb");
  if (!save) return state = false;

  size_t count = fwrite(record, sizeof(int), 1, save);
  fclose(save);

  if (!count) state = false;
  return state;
}

void updateLevel(params_t *prms) {
  int current_level = prms->stats->score / POINTS_PER_LEVEL;
  if (current_level > 10) current_level = 10;
  prms->stats->level = current_level;
}

void countPoints(params_t *prms, int lines) {
  if (!lines) return;
  int points_number = kScoreForCompleteLiens[lines - 1];
  prms->stats->score += points_number;
  if (prms->stats->score >= POINTS_PER_LEVEL) updateLevel(prms);
  if (prms->stats->score > prms->stats->record) {
    prms->stats->record = prms->stats->score;
    saveHighestScore(&prms->stats->record, RECORDS_FILE);
  }
}

void attaching(params_t *prms) {
  int lines_destroyed = 0;
  for (int row = prms->stats->rows - 1; row >= 0; row--) {
    if (lineFull(prms->stats, row)) {
      shiftDown(prms->stats, row);
      row++;
      lines_destroyed++;
    }
  }
  countPoints(prms, lines_destroyed);
  *prms->state = SPAWN;
}

void resetData(params_t *prms) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      prms->stats->board[i][j] = 0;
    }
  }
  prms->stats->level = 0;
  prms->stats->score = 0;
}

void gameOver(params_t *prms) {
  *prms->game_over = TRUE;
  printEnd(prms->w.end);
  resetData(prms);
  *prms->state = START;
}

action fsm_table[7][8] = {
    {NULL, NULL, NULL, NULL, NULL, exitstate, spawn, NULL},
    {spawn, spawn, spawn, spawn, spawn, spawn, spawn},
    {shifting, moveDown, moveRight, moveLeft, rotate, exitstate, NULL,
     pauseGame},
    {attaching, attaching, attaching, attaching, attaching, attaching,
     attaching, attaching},
    {gameOver, gameOver, gameOver, gameOver, gameOver, gameOver, gameOver},
    {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
     exitstate},
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, pauseGame}};

bool checkBounds(game *tetris, int row, int column) {
  return row >= 0 && column >= 0 && row < tetris->rows && column < tetris->cols;
}

bool tetrominoFits(game *tetris, tetris_block block) {
  bool fits = true;
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    tetris_location cell = TETRIS_FIGURE[block.type][block.orient][i];
    int r = block.coordinates.row + cell.row;
    int c = block.coordinates.col + cell.col;
    if (!checkBounds(tetris, r, c) || getCell(tetris, r, c) != 0) {
      fits = false;
    }
  }
  return fits;
}

GameInfo_t updateCurrentState(game *tetris, GameInfo_t *graphics) {
  GameInfo_t *data = graphics;
  TRANSFER_DATA(BOARD_HEIGHT, BOARD_WIDTH, data->field, tetris->board);
  TRANSFER_DATA(TETROMINO_SIZE, TETROMINO_SIZE, data->next,
                tetris->next_figure);
  data->level = tetris->level;
  data->score = tetris->score;
  data->high_score = tetris->record;
  return *data;
}

int getCell(game *tetris, int row, int column) {
  return tetris->board[row][column];
}

void setCell(game *tetris, int row, int column, int value) {
  tetris->board[row][column] = value;
}

void randomSortArray(int *arr) {
  int temp = 0, j = 0;
  for (int i = NUM_TETROMINOES - 1; i > 0; i--) {
    j = rand() % (i + 1);
    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
  // for (int i = 0; i < NUM_TETROMINOES; i++) printw("%d", arr[i]);
}

int randomTetromino() {
  static int tetrominos_array[NUM_TETROMINOES] = {0};
  static int n = -1;
  n++;
  if (n >= NUM_TETROMINOES) n = 0;
  if (n == 0) {
    for (int i = 0; i < NUM_TETROMINOES; i++) {
      tetrominos_array[i] = i;
    }
    randomSortArray(tetrominos_array);
  }
  // printw(" n%d p%d ", n, tetrominos_array[n]);
  return tetrominos_array[n];
}

void newFallingFigure(game *tetris) {
  tetris->falling_tetromino = tetris->next_tetromino;
  tetris->next_tetromino.type = randomTetromino();
  tetris->next_tetromino.orient = 0;
  tetris->next_tetromino.coordinates.row = 0;
  tetris->next_tetromino.coordinates.col = tetris->cols / 2 - 2;
  removeNextTetromino(tetris, tetris->next_tetromino);
  placeNextTetromino(tetris, tetris->next_tetromino);
}

void gameInit(game *tetris, int rows, int cols) {
  tetris->rows = rows;
  tetris->cols = cols;
  tetris->level = 0;
  loadHighestScore(&tetris->record, RECORDS_FILE);
  tetris->tick_till_drop = GRAVITY_LEVEL[tetris->level];
  srand(time(NULL));
  newFallingFigure(tetris);
}

void placeTetromino(game *tetris, tetris_block piece) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    tetris_location cell = TETRIS_FIGURE[piece.type][piece.orient][i];
    setCell(tetris, piece.coordinates.row + cell.row,
            piece.coordinates.col + cell.col, piece.type + 1);
  }
}

void placeNextTetromino(game *tetris, tetris_block piece) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    tetris_location cell = TETRIS_FIGURE[piece.type][piece.orient][i];
    tetris->next_figure[cell.row][cell.col] = piece.type + 1;
  }
}

void removeNextTetromino(game *tetris, tetris_block piece) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      tetris->next_figure[i][j] = 0;
    }
  }
}

void removeTetromino(game *tetris, tetris_block piece) {
  for (int i = 0; i < 4; i++) {
    tetris_location cell = TETRIS_FIGURE[piece.type][piece.orient][i];
    setCell(tetris, piece.coordinates.row + cell.row,
            piece.coordinates.col + cell.col, 0);
  }
}

void checkData(WINDOW *w, int **field) {
  int i, j;
  for (i = 0; i < BOARD_HEIGHT; i++) {
    for (j = 0; j < BOARD_WIDTH; j++) {
      mvwprintw(w, i + 1, j + 1, "%d", field[i][j]);
    }
    mvwprintw(w, i + 1, j + 1, "\n");
  }
  wrefresh(w);
}

void printCoord(WINDOW *sidebar, game *tetris) {
  wprintw(sidebar, "%d %d ", tetris->falling_tetromino.coordinates.row,
          tetris->falling_tetromino.coordinates.col);
  wrefresh(sidebar);
}

Signals_t get_signal(int user_input) {
  Signals_t rc = None;

  if (user_input == KEY_DOWN)
    rc = Down;
  else if (user_input == KEY_LEFT)
    rc = Left;
  else if (user_input == KEY_RIGHT)
    rc = Right;
  else if (user_input == ESC_KEY)
    rc = ESC;
  else if (user_input == ENTER_KEY)
    rc = Enter;
  else if (user_input == 'p')
    rc = Pause;
  else if (user_input == 'r' || user_input == KEY_UP)
    rc = Rotate;

  return rc;
}

void sigact(Signals_t signal, tetris_state *state, game *tetris,
            bool *game_over_flag) {
  params_t prms;
  prms.stats = tetris;
  prms.state = state;
  prms.game_over = game_over_flag;
  prms.w.pause = createNewWindow(prms.w.pause, BOARD_WIDTH * 2, BOARDS_BEGIN);
  prms.w.start = createNewWindow(prms.w.start, BOARD_WIDTH * 2 + HUD_WIDTH + 2,
                                 BOARDS_BEGIN);
  prms.w.end = createNewWindow(prms.w.end, BOARD_WIDTH * 2 + HUD_WIDTH + 2,
                               BOARDS_BEGIN);

  if (*prms.state == START && *prms.game_over == FALSE)
    printStart(prms.w.start);

  action act = fsm_table[*state][signal];

  if (act) act(&prms);

  if (*prms.state == EXIT_STATE) {
    delwin(prms.w.pause);
    delwin(prms.w.start);
    delwin(prms.w.end);
  }
}

void graphicProcessing(WINDOW *board, WINDOW *sidebar, GameInfo_t *data,
                       tetris_state *state) {
  if (*state == MOVING) {
    displayField(board, data);
    displayNextFigure(sidebar, data);
    printStats(sidebar, data);
  }
  if (*state == GAMEOVER) wclear(sidebar);
}

void gameLoop(windows *w, game *tetris, GameInfo_t *data) {
  bool running = TRUE;
  bool game_over = FALSE;
  int signal = 0;
  tetris_state state = START;

  while (running) {
    signal = getch();
    sigact(get_signal(signal), &state, tetris, &game_over);
    graphicProcessing(w->board, w->sidebar, data, &state);
    *data = updateCurrentState(tetris, data);
    refresh();
    if (state == EXIT_STATE) running = FALSE;
    delay_output(5);
  }
}

int main() {
  WIN_INIT(1);
  windows w;
  game tetris = {0};
  GameInfo_t tetris_graphics = {0};

  w.board = createNewWindow(w.board, BOARD_WIDTH * 2, BOARDS_BEGIN);
  w.sidebar =
      createNewWindow(w.sidebar, HUD_WIDTH, BOARDS_BEGIN + BOARD_WIDTH * 2 + 2);

  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  gameLoop(&w, &tetris, &tetris_graphics);

  endwin();

  return 0;
}

const tetris_location TETRIS_FIGURE[NUM_TETROMINOES][NUM_ORIENTATIONS]
                                   [TETROMINO_SIZE] = {
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

int GRAVITY_LEVEL[19 + 1] = {
    // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    50, 48, 46, 44, 42, 40, 38, 36, 34, 32,
    // 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    30, 28, 26, 24, 22, 20, 16, 12, 8, 4};