if (tetris->falling_tetromino.coordinates.row != 17 &&
    tetris->tick_till_drop++ == 14) {
  removeTetromino(tetris, tetris->falling_tetromino);
  tetris->falling_tetromino.coordinates.row++;
}
if (tetris->tick_till_drop == 15) tetris->tick_till_drop = 0;

if (signal == KEY_RIGHT &&
    tetris->falling_tetromino.coordinates.col != BOARD_WIDTH - 3) {
  removeTetromino(tetris, tetris->falling_tetromino);
  tetris->falling_tetromino.coordinates.col++;
  printCoord(sidebar, tetris);
}
if (signal == KEY_LEFT && tetris->falling_tetromino.coordinates.col != -1) {
  removeTetromino(tetris, tetris->falling_tetromino);
  tetris->falling_tetromino.coordinates.col--;
  printCoord(sidebar, tetris);
}
if (signal == KEY_DOWN && tetris->falling_tetromino.coordinates.row < 17) {
  removeTetromino(tetris, tetris->falling_tetromino);
  tetris->falling_tetromino.coordinates.row++;
  printCoord(sidebar, tetris);
}
if (signal == KEY_UP && tetris->falling_tetromino.coordinates.row != 0) {
  removeTetromino(tetris, tetris->falling_tetromino);
  tetris->falling_tetromino.coordinates.row--;
  printCoord(sidebar, tetris);
}
if (signal == 'r') {
  removeTetromino(tetris, tetris->falling_tetromino);
  tetris->falling_tetromino.orient++;
  printw("%d", tetris->falling_tetromino.orient);
  if (tetris->falling_tetromino.orient == 4)
    tetris->falling_tetromino.orient = 0;
  printCoord(sidebar, tetris);
}