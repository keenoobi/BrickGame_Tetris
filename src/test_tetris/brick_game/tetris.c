#include "tetris.h"

#include <stdlib.h>
#include <time.h>

int getCell(game *tetris, int row, int column) {
  return tetris->board[row][column];
}

static int randomTetromino() { return rand() % NUM_TETROMINOES; };

void newFallingFigure(game *tetris) {
  tetris->falling_tetromino = tetris->next_tetromino;
  tetris->next_tetromino.type = randomTetromino();
  tetris->next_tetromino.orient = 0;
  tetris->next_tetromino.coordinates.row = 0;
  tetris->next_tetromino.coordinates.col = tetris->cols / 2 - 2;
};

int **allocateBoard(int height, int width) {
  int **board = calloc(height, sizeof(int *) + width * sizeof(int));
  if (board != NULL) {
    board[0] = (int *)(board + height);
    for (int i = 1; i < height; i++) {
      board[i] = board[0] + i + width;
    }
  }
  return board;
};

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

bool checkEndGame(game *) { return true; }

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
  newFallingFigure(new);
  new->next_tetromino.coordinates.col = new->cols / 2 - 2;

  return new;
};
void printtetris(WINDOW *w) {
  box(w, 0, 0);
  // refresh();
  // wrefresh(w);
}

void printFigure(WINDOW *w) {
  mvwprintw(w, 5, 5, "@");
  // refresh();
  // wrefresh(w);
}

int main() {
  WIN_INIT(1);
  WINDOW *board, *sidebar;
  game *tetris;
  bool running = true;
  int endgame = 1;
  tetris = gameInit(BOARD_HEIGHT, BOARD_WIDTH);
  board = newwin(BOARD_HEIGHT + 1, BOARD_WIDTH + 1, BOARDS_BEGIN, BOARDS_BEGIN);
  sidebar = newwin(BOARD_HEIGHT + 1, HUD_WIDTH + 1, BOARDS_BEGIN,
                   BOARDS_BEGIN + 1 + BOARD_WIDTH);
  // printBoard(board, sidebar);
  while (endgame) {
    int ch = getch();
    if (ch == 27 || ch == 113) endgame = 0;
    // if (ch == 'p') printw("%d", CONVERT_TO_CELL(randomTetromino()));
    // running = check_end_game(tetris);

    // print_figure();
    halfdelay(10);
  }

  freeGame(tetris);

  wclear(stdscr);
  endwin();

  // board = newwin(BOARD_HEIGHT + 1, BOARD_WIDTH + 1, BOARDS_BEGIN,
  // BOARDS_BEGIN); sidebar = newwin(BOARD_HEIGHT + 1, HUD_WIDTH + 1,
  // BOARDS_BEGIN,
  //                  BOARDS_BEGIN + 1 + BOARD_WIDTH);

  // box(board, 0, 0);
  // box(sidebar, 0, 0);

  // // mvwprintw(board, 1, 1, "@");
  // mvwprintw(sidebar, 1, 2, "Score");
  // mvwprintw(sidebar, 4, 2, "Record");
  // mvwprintw(sidebar, 7, 2, "Level");
  // mvwprintw(sidebar, 10, 2, "Next");
  // // print_stats(sidebar);
  // refresh();
  // wrefresh(board);
  // wrefresh(sidebar);

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
     {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
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