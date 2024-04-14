#ifndef _TEST_H_
#define _TEST_H_

#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/tetris.h"

Suite *tetromino_suite(void);
Suite *api_suite(void);
Suite *score_suite(void);
Suite *record_suite(void);
Suite *helpers_suite(void);

#endif