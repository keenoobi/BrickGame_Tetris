#include "cli.h"

void print_board() {
  WINDOW *board, *sidebar;
  board = newwin(BOARD_N + 1, BOARD_M + 1, BOARDS_BEGIN, BOARDS_BEGIN);
  sidebar = newwin(BOARD_N + 1, HUD_WIDTH + 1, BOARDS_BEGIN,
                   BOARDS_BEGIN + 1 + BOARD_M);

  refresh();
  box(board, 0, 0);
  box(sidebar, 0, 0);

  mvwprintw(board, 1, 1, "@");
  wrefresh(board);
  wrefresh(sidebar);
}

void allocate_field(GameInfo_t *game) {}