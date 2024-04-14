#include "tetris.h"

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

void graphicProcessing(WINDOW *board, WINDOW *sidebar, GameInfo_t *data,
                       tetris_state *state) {
  if (*state == MOVING) {
    displayField(board, data);
    displayNextFigure(sidebar, data);
    printStats(sidebar, data);
  }
  if (*state == GAMEOVER) wclear(sidebar);
}