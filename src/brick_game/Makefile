CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
CURSES=-lncurses
DIR=
MAIN_PROJECT=$(wildcard **/**/*.c **/*.c ../gui/**/*.c ../backend_tests/*.c) # list of all the C source files in the project
SOURCES=$(filter-out ../backend_tests/%.c, $(MAIN_PROJECT))
TEST_SOURCES=$(filter-out tetris/tetris.c ../gui/cli/tetris_frontend.c tetris/fsm/fsm_matrix.c, $(MAIN_PROJECT))
MAIN=tetris_game
TEST_RESULT=../backend_tests/test
OBJECTS=$(SOURCES:.c=.o)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)
RESULT=tetris_game.a

OS := $(shell uname)
ifeq ($(OS),Linux)
	OPEN= xdg-open report/index.html
	CHECK_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
endif

ifeq ($(OS),Darwin)
	OPEN= open report/index.html
	CHECK_LIB = -lcheck
endif

all: $(RESULT)

install: $(RESULT)
	$(CC) $(CFLAGS) $^ $(CURSES) -o $(DIR)$(MAIN)

test: $(TEST_OBJECTS) 
	# clear
	$(CC) $(CFLAGS) $^ -o $(TEST_RESULT) $(CHECK_LIB)
	./$(TEST_RESULT)

$(RESULT): $(OBJECTS)
	ar rc $(RESULT) $(OBJECTS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

gcov_report: $(TEST_SOURCES) 
	$(CC) $(CFLAGS) --coverage $^ $(CHECK_LIB) -o $@
	chmod +x *
	./$@
	lcov -t "$@" -o $@.info --no-external -c -d .
	genhtml -o report/ $@.info
	$(OPEN)

clang:
	clang-format -style=Google -n $(MAIN_PROJECT)

git:
	git add .
	git commit
	git push origin develop

clean:
	rm -rf $(RESULT) $(OBJECTS) $(TEST_OBJECTS) $(TEST_RESULT) gcov_report* *.gc* report/ *.o $(DIR)$(MAIN)