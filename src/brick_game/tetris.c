#include "tetris.h"

#define ESC 27
#define Q 113

int main() {
  test a = {1, 1};

  WIN_INIT(1);
  //   setlocale(LC_ALL, "");
  print_overlay();
  // game_loop();
  int endgame = 1;
  int counter = 0;
  while (endgame) {
    print_figure(&a);
    int ch = getch();

    if (a.pos_y != BOARD_HEIGHT && counter++ == 9) {
      clear_figure(&a);
      a.pos_y++;
    }
    if (counter == 10) counter = 0;
    if (ch == KEY_RIGHT && a.pos_x != BOARD_WIDTH - 3) {
      clear_figure(&a);
      a.pos_x++;
    }
    if (ch == KEY_LEFT && a.pos_x != 1) {
      clear_figure(&a);
      a.pos_x--;
    }
    if (ch == KEY_UP && a.pos_y != 1) {
      clear_figure(&a);
      a.pos_y--;
    }
    if (ch == KEY_DOWN && a.pos_y != BOARD_HEIGHT) {
      clear_figure(&a);
      a.pos_y++;
    }

    if (ch == ESC || ch == Q) endgame = 0;

    delay_output(1000);
  }

  endwin();

  return 0;
}