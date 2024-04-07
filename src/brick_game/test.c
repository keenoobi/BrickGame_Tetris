#include <ncurses.h>

// int main() {
//   // Инициализация ncurses
//   initscr();
//   cbreak();
//   noecho();
//   curs_set(0);

//   // Определение массива для квадрата (4x4)
//   // int square[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1,
//   // 0}};
//   int square[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1,
//   0}};

//   // Отображение квадрата на экране
//   for (int i = 0; i < 4; i++) {
//     for (int j = 0; j < 4; j++) {
//       if (square[i][j] == 1) {
//         mvaddch(j, 3 - i,
//                 ACS_BLOCK);  // Умножаем j на 2 для учета ширины символов в
//                              // терминале
//       }
//     }
//   }

//   refresh();

//   // Ожидание нажатия клавиши перед завершением программы
//   getch();

//   // Завершение работы с ncurses
//   endwin();

//   return 0;
// }

#include <ncurses.h>

void rotateSquare(int square[4][4]) {
  int temp[4][4];

  // Поворот квадрата на 90 градусов по часовой стрелке
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[j][3 - i] = square[i][j];
    }
  }

  // Копирование повернутого квадрата обратно в исходный массив
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      square[i][j] = temp[i][j];
    }
  }
}

int main() {
  // Инициализация ncurses
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  // Определение массива для квадрата (4x4)
  int square[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}};
  // int square[4][4] = {{1, 1, 1, 1}};

  int ch;

  do {
    clear();

    // Отображение квадрата на экране
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (square[i][j] == 1) {
          mvaddch(i, j,
                  ACS_BLOCK);  // Умножаем j на 2 для учета ширины символов в
                               // терминале
        }
      }
    }

    refresh();

    ch = getch();

    if (ch == 'r' || ch == 'R') {
      rotateSquare(square);
    }

  } while (ch != 'q' && ch != 'Q');  // Для выхода из цикла нажмите 'q' или 'Q'

  // Завершение работы с ncurses
  endwin();

  return 0;
}