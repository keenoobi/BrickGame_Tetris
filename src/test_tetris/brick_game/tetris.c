#include "tetris.h"

// #include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
  if (*prms->state != PAUSE)
    *prms->state = PAUSE;
  else
    *prms->state = MOVING;
}

void checkSides(params_t *prms) {
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col >= 7)
    prms->stats->falling_tetromino.coordinates.col--;
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col >= 7)
    prms->stats->falling_tetromino.coordinates.col--;
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col <= 2)
    prms->stats->falling_tetromino.coordinates.col++;
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino) &&
      prms->stats->falling_tetromino.coordinates.col <= 2)
    prms->stats->falling_tetromino.coordinates.col++;
}

void rotate(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  if (prms->stats->falling_tetromino.type != 3) {
    prms->stats->falling_tetromino.orient++;
  }
  if (prms->stats->falling_tetromino.orient == 4) {
    prms->stats->falling_tetromino.orient = 0;
  }
  int tmp_col = prms->stats->falling_tetromino.coordinates.col;
  int tmp_ori = prms->stats->falling_tetromino.orient;

  checkSides(prms);

  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    prms->stats->falling_tetromino.coordinates.col = tmp_col;
    prms->stats->falling_tetromino.orient = tmp_ori - 1;
    if (prms->stats->falling_tetromino.orient < 0)
      prms->stats->falling_tetromino.orient = 3;
  }
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
      // setCell(tetris, i, j, 0);
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
  if (prms->stats->score > POINTS_PER_LEVEL) updateLevel(prms);
  if (prms->stats->score > prms->stats->record) {
    prms->stats->record = prms->stats->score;
    printw("%d", saveHighestScore(&prms->stats->record, RECORDS_FILE));
  }
}

void attaching(params_t *prms) {
  // removeTetromino(prms->stats, prms->stats->falling_tetromino);
  int lines_destroyed = 0;
  for (int i = prms->stats->rows - 1; i >= 0; i--) {
    if (lineFull(prms->stats, i)) {
      shiftDown(prms->stats, i);
      i++;
      lines_destroyed++;
    }
  }
  countPoints(prms, lines_destroyed);
  // placeTetromino(prms->stats, prms->stats->falling_tetromino);
  *prms->state = SPAWN;
}

void gameOver(params_t *prms) { exit(EXIT_FAILURE); }

//
//
//
//
//

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

//
//
//
//
//

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

GameInfo_t *updateCurrentState(game *tetris) {
  GameInfo_t *data = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  data->field = tetris->board;
  data->next = tetris->next_figure;
  data->level = tetris->level;
  data->score = tetris->score;
  data->high_score = tetris->record;

  return data;
}

int getCell(game *tetris, int row, int column) {
  return tetris->board[row][column];
}

void setCell(game *tetris, int row, int column, int value) {
  tetris->board[row][column] = value;
}

static int randomTetromino() { return rand() % NUM_TETROMINOES; };

void newFallingFigure(game *tetris) {
  tetris->falling_tetromino = tetris->next_tetromino;
  tetris->next_tetromino.type = randomTetromino();
  tetris->next_tetromino.orient = 0;
  tetris->next_tetromino.coordinates.row = 0;
  tetris->next_tetromino.coordinates.col = tetris->cols / 2 - 2;
  removeNextTetromino(tetris, tetris->next_tetromino);
  placeNextTetromino(tetris, tetris->next_tetromino);
}

int **allocateBoard(int height, int width) {
  int **board = (int **)calloc(height, sizeof(int *) + width * sizeof(int));
  if (board != NULL) {
    board[0] = (int *)(board + height);
    for (int i = 1; i < height; i++) {
      board[i] = board[0] + i * width;
    }
  }
  return board;
}

void freeBoard(game *tetris) {
  if (tetris->board) {
    free(tetris->board);
  }
  if (tetris->next_figure) free(tetris->next_figure);
}

void freeGame(game *tetris) {
  if (tetris) {
    freeBoard(tetris);
    free(tetris);
  }
}

bool checkEndGame(game *) { return true; }  //нужно дописать

game *gameInit(int rows, int cols) {
  game *new = (game *)malloc(sizeof(game));
  new->rows = rows;
  new->cols = cols;
  new->board = allocateBoard(new->rows, new->cols);
  new->next_figure = allocateBoard(TETROMINO_SIZE, TETROMINO_SIZE);
  new->score = 0;
  new->record = 0;
  loadHighestScore(&new->record, RECORDS_FILE);
  // if (loadHighestScore(&new->record, RECORDS_FILE)) new->record = 0;
  new->level = 0;
  new->tick_till_drop = GRAVITY_LEVEL[new->level];
  new->points_remaining = POINTS_PER_LEVEL;
  srand(time(NULL));
  new->next_tetromino.coordinates.col = new->cols / 2 - 2;
  newFallingFigure(new);
  newFallingFigure(new);

  return new;
}

void printtetris(WINDOW *w) {
  box(w, 0, 0);
  // refresh();
  // wrefresh(w);
}

void printFigure(WINDOW *w) {
  mvwprintw(w, 5, 5, "@");
  // refresh();
  wrefresh(w);
}

WINDOW *createNewWindow(WINDOW *w, int width, int x) {
  w = newwin(BOARD_HEIGHT + 2, width + 2, BOARDS_BEGIN, x);
  return w;
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
  for (i = 0; i < 20; i++) {
    for (j = 0; j < 10; j++) {
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

void sigact(Signals_t signal, tetris_state *state, game *tetris) {
  params_t prms;
  prms.stats = tetris;
  prms.state = state;
  // printw("%d ", *state);

  action act = fsm_table[*state][signal];

  if (act) act(&prms);
}

void userInput(Signals_t action, bool hold) {}

void gameLoop(WINDOW *board, WINDOW *sidebar, game *tetris, GameInfo_t *data) {
  bool running = TRUE;
  int signal = 0;
  bool hold = FALSE;
  tetris_state state = START;

  while (running) {
    signal = getch();

    sigact(get_signal(signal), &state, tetris);

    // checkData(board, tetris->board);

    if (state == MOVING) displayField(board, data);
    if (state == MOVING) displayNextFigure(sidebar, data);
    if (state == MOVING) printStats(sidebar, data);

    free(data);
    data = updateCurrentState(tetris);

    refresh();
    if (state == GAMEOVER || state == EXIT_STATE) running = FALSE;
    delay_output(5);
    // halfdelay(10);
  }
  free(data);
}

int main() {
  WIN_INIT(1);
  WINDOW *board, *sidebar;
  game *tetris;
  GameInfo_t *tetris_data;
  init_colors();

  board = createNewWindow(board, BOARD_WIDTH, BOARDS_BEGIN);
  sidebar = createNewWindow(sidebar, HUD_WIDTH, BOARDS_BEGIN + BOARD_WIDTH + 2);
  tetris = gameInit(BOARD_HEIGHT, BOARD_WIDTH);
  tetris_data = gameStateInit(BOARD_HEIGHT, BOARD_WIDTH);

  printBoard(board, sidebar);

  gameLoop(board, sidebar, tetris, tetris_data);

  freeGame(tetris);
  freeGameInfo(tetris_data);

  wclear(board);
  wclear(sidebar);
  wclear(stdscr);

  delwin(board);
  delwin(sidebar);
  delwin(stdscr);

  endwin();

  //   int height, width, start_x, start_y;
  //   height = 21;
  //   width = 11;
  //   start_x = 2;
  //   start_y = 2;
  //   start_color();
  //   init_pair(1, 0, 5);
  //   bkgd(COLOR_PAIR(1));

  // getchar();

  return 0;
}

const tetris_location TETRIS_FIGURE[7][4][4] = {
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

// if (ch == 'p') printw("%d", CONVERT_TO_CELL(randomTetromino()));
// running = check_end_game(tetris);