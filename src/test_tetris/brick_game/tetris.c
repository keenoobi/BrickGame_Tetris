#include "tetris.h"

int main() {
  WIN_INIT(1);

  print_board();
  //   int height, width, start_x, start_y;
  //   height = 21;
  //   width = 11;
  //   start_x = 2;
  //   start_y = 2;
  //   start_color();
  //   init_pair(1, 0, 5);
  //   bkgd(COLOR_PAIR(1));

  // getchar();
  while (getch() != 'q') {
    print_figure();
  }

  endwin();

  return 0;
}