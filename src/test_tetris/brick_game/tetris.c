#include "tetris.h"

// #include <stdlib.h>
#include <time.h>

void exitstate(params_t *prms) { *prms->state = EXIT_STATE; }

void spawn(params_t *prms) {
  if (!tetrominoFits(prms->stats, prms->stats->falling_tetromino))
    *prms->state = GAMEOVER;
  else {
    newFallingFigure(prms->stats);
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
    *prms->state = MOVING;
  }
}

void moveDown(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.coordinates.row++;
  if (tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    *prms->state = SHIFTING;
  } else {
    prms->stats->falling_tetromino.coordinates.row--;
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
    *prms->state = SPAWN;
  }
}

void moveRight(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.coordinates.col++;
  if (tetrominoFits(prms->stats, prms->stats->falling_tetromino)) {
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
    *prms->state = SHIFTING;
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
    *prms->state = SHIFTING;
  } else {
    prms->stats->falling_tetromino.coordinates.col++;
    placeTetromino(prms->stats, prms->stats->falling_tetromino);
  }
}

void pauseGame(params_t *prms) {
  if (*prms->state != PAUSE)
    *prms->state = PAUSE;
  else
    *prms->state = SHIFTING;
}

void rotate(params_t *prms) {
  removeTetromino(prms->stats, prms->stats->falling_tetromino);
  prms->stats->falling_tetromino.orient++;
  if (prms->stats->falling_tetromino.orient == 4) {
    prms->stats->falling_tetromino.orient = 0;
    prms->state = SHIFTING;
  }
}

void shifting(params_t *prms) {
  if (prms->stats->tick_till_drop-- <= 0) {
    prms->stats->tick_till_drop = GRAVITY_LEVEL[prms->stats->level];
    moveDown(prms);
  }
}

action fsm_table[8][8] = {
    {NULL, NULL, NULL, NULL, exitstate, spawn, NULL},
    {spawn, spawn, spawn, spawn, spawn, spawn, spawn},
    {/* moveup */ NULL, moveDown, moveRight, moveLeft, exitstate, rotate,
     pauseGame},
    {shifting, shifting, shifting, shifting, shifting, shifting, shifting},
    {reach, reach, reach, reach, reach, reach, reach},
    {collide, collide, collide, collide, collide, collide, collide},
    {gameover, gameover, gameover, gameover, gameover, gameover, gameover},
    {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
     exitstate},
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, pauseGame},
};

//
//
//
//
//

bool checkBounds(game *tetris, int row, int column) {
  return row >= 0 && column >= 0 && row < tetris->rows && column < tetris->cols;
}

bool tetrominoFits(game *tetris, tetris_block block) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    tetris_location cell = TETRIS_FIGURE[block.type][block.orient][i];
    int r = block.coordinates.row + cell.row;
    int c = block.coordinates.col + cell.col;
    if (!checkBounds(tetris, r, c) || getCell(tetris, r, c) != 0) {
      return false;
    }
  }
  return true;
}

GameInfo_t *updateCurrentState(game *tetris) {
  GameInfo_t *data = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  data->field = tetris->board;

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
  new->points = 0;
  new->level = 0;
  new->tick_till_drop = GRAVITY_LEVEL[new->level];
  new->points_remaining = POINTS_PER_LEVEL;
  srand(time(NULL));
  newFallingFigure(new);
  new->next_tetromino.coordinates.col = new->cols / 2 - 2;

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
void userInput(UserAction_t action, bool hold) {}

void finiteStateMachine(UserAction_t sig, tetris_state *state, game *tetris) {
  switch (*state) {
    case START:
      switch (sig) {
        case Start:
          *state = SPAWN;
          break;
        case Terminate:
          *state = EXIT_STATE;
          break;
        case Pause:
          *state = PAUSE;
        default:
          *state = START;
          break;
      }
      break;
    case SPAWN:
      break;
    case MOVING:
      switch (sig) {
        case Left:
          /* code */
          break;
        case Right:
          /* code */
          break;
        case Down:
          /* code */
          break;
        case Action:
          *state = SHIFTING;
          /* code */
          break;
        default:
          break;
      }
    case SHIFTING:
      /* code */
      break;
    case ATTACHING:
      /* code */
      break;
    case GAMEOVER:
      /* code */
      break;

    default:
      break;
  }
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
    tetris->next->next[cell.row][cell.col] = piece.type + 1;
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

void gameLoop(WINDOW *board, WINDOW *sidebar, game *tetris, GameInfo_t *data) {
  bool running = TRUE;
  UserAction_t signal = 8;
  bool hold = FALSE;
  tetris_state state = START;

  // int endgame = 1;  //временная переменная

  while (running) {
    signal = getch();
    placeTetromino(tetris, tetris->falling_tetromino);
    // checkData(board, tetris->board);

    displayField(board, data);
    if (state == GAMEOVER || state == EXIT_STATE) running = FALSE;
    if (signal == 'p') printw("%d", CONVERT_TO_CELL(randomTetromino()));
    // userInput(&signal, hold);
    // finiteStateMachine(&signal, &state, tetris);
    // printFigure(board);

    if (signal == 27 || signal == 113) running = FALSE;  //временное
    data = updateCurrentState(tetris);
    free(data);

    // newFallingFigure(tetris);
    delay_output(50);
    // halfdelay(10);
  }
}

int main() {
  WIN_INIT(1);
  WINDOW *board, *sidebar;
  game *tetris;
  GameInfo_t *tetris_data;
  // bool running = true;

  board = createNewWindow(board, BOARD_WIDTH, BOARDS_BEGIN);
  sidebar = createNewWindow(sidebar, HUD_WIDTH, BOARDS_BEGIN + BOARD_WIDTH + 2);
  tetris = gameInit(BOARD_HEIGHT, BOARD_WIDTH);
  tetris_data = gameStateInit(BOARD_HEIGHT, BOARD_WIDTH);
  printBoard(board, sidebar);

  gameLoop(board, sidebar, tetris, tetris_data);

  freeGame(tetris);
  freeGameInfo(tetris_data);

  wclear(stdscr);
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

tetris_location TETRIS_FIGURE[7][4][4] = {
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