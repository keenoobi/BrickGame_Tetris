#include "tetris.h"

int main() {
  WIN_INIT(1);
  //   setlocale(LC_ALL, "");
  print_overlay();
  // game_loop();
  while (getch() != 27)
    ;

  endwin();

  return 0;
}