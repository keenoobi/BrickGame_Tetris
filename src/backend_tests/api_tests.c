#include "test.h"

START_TEST(api_test0) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  GameInfo_t front_data = {0};
  front_data = updateCurrentState(&tetris, &front_data);

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      ck_assert_int_eq(tetris.board[i][j], front_data.field[i][j]);
      if (i < 4 && j < 4)
        ck_assert_int_eq(tetris.next_figure[i][j], front_data.next[i][j]);
    }
  }
}
END_TEST

START_TEST(api_test1) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  GameInfo_t front_data = {0};
  front_data = updateCurrentState(&tetris, &front_data);
  ck_assert_int_eq(tetris.level, front_data.level);
  ck_assert_int_eq(tetris.score, front_data.score);
  ck_assert_int_eq(tetris.record, front_data.high_score);
}
END_TEST

START_TEST(api_test2) {
  game tetris = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  tetris.level = 5;
  tetris.score = 600;
  tetris.record = 3500;
  GameInfo_t front_data = {0};
  front_data = updateCurrentState(&tetris, &front_data);
  ck_assert_int_eq(tetris.level, front_data.level);
  ck_assert_int_eq(tetris.score, front_data.score);
  ck_assert_int_eq(tetris.record, front_data.high_score);
}
END_TEST

START_TEST(api_loop_test0) {
  game tetris = {0};
  GameInfo_t front_data = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  newFallingFigure(&tetris);
  placeTetromino(&tetris, tetris.falling_tetromino);
  placeNextTetromino(&tetris, tetris.next_tetromino);
  front_data = updateCurrentState(&tetris, &front_data);

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      ck_assert_int_eq(tetris.board[i][j], front_data.field[i][j]);
      if (i < 4 && j < 4)
        ck_assert_int_eq(tetris.next_figure[i][j], front_data.next[i][j]);
    }
  }
}
END_TEST

START_TEST(api_loop_test1) {
  game tetris = {0};
  GameInfo_t front_data = {0};
  gameInit(&tetris, BOARD_HEIGHT, BOARD_WIDTH);
  newFallingFigure(&tetris);
  placeTetromino(&tetris, tetris.falling_tetromino);
  placeNextTetromino(&tetris, tetris.next_tetromino);
  front_data = updateCurrentState(&tetris, &front_data);

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      ck_assert_int_eq(tetris.board[i][j], front_data.field[i][j]);
      if (i < 4 && j < 4)
        ck_assert_int_eq(tetris.next_figure[i][j], front_data.next[i][j]);
    }
  }

  removeTetromino(&tetris, tetris.falling_tetromino);
  removeNextTetromino(&tetris);
  front_data = updateCurrentState(&tetris, &front_data);

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      ck_assert_int_eq(tetris.board[i][j], front_data.field[i][j]);
      if (i < 4 && j < 4)
        ck_assert_int_eq(tetris.next_figure[i][j], front_data.next[i][j]);
    }
  }
}
END_TEST

Suite *api_suite(void) {
  Suite *s;
  TCase *tc_main;

  s = suite_create("api tests");
  tc_main = tcase_create("main");

  tcase_add_test(tc_main, api_test0);
  tcase_add_test(tc_main, api_test1);
  tcase_add_test(tc_main, api_test2);

  tcase_add_loop_test(tc_main, api_loop_test0, 0, 50);
  tcase_add_loop_test(tc_main, api_loop_test1, 0, 50);

  suite_add_tcase(s, tc_main);
  return s;
}