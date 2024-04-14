CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
CURSES=-lncurses
DIR=
DIST_DIR=archive
MAIN_PROJECT=$(wildcard **/**/*.c **/**/**/*.c gui/**/*.c backend_tests/*.c) # list of all the C source files in the project
SOURCES=$(filter-out backend_tests/%.c, $(MAIN_PROJECT))
TEST_SOURCES=$(filter-out **/tetris/tetris.c gui/cli/tetris_frontend.c **/tetris/fsm/fsm_matrix.c, $(MAIN_PROJECT))
MAIN=tetris.game
TEST_RESULT=../backend_tests/test
OBJECTS=$(SOURCES:.c=.o)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)
RESULT=tetris_game.a

OS := $(shell uname)
ifeq ($(OS),Linux)
	OPEN_COMMAND= xdg-open
	OPEN= $(OPEN_COMMAND) report/index.html
	CHECK_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
endif

ifeq ($(OS),Darwin)
	OPEN_COMMAND= open
	OPEN= $(OPEN_COMMAND) report/index.html
	CHECK_LIB = -lcheck
endif

all: rungame

install: $(RESULT)
	$(CC) $(CFLAGS) $^ $(CURSES) -o $(DIR)$(MAIN)

uninstall: clean
	rm -f $(DIR)$(MAIN)
	rm -f $(DIR)record.save

test: $(TEST_OBJECTS) 
	$(CC) $(CFLAGS) $^ -o $(TEST_RESULT) $(CHECK_LIB)
	./$(TEST_RESULT)

$(RESULT): $(OBJECTS)
	ar rc $(RESULT) $(OBJECTS)

dvi:
	$(OPEN_COMMAND) ../dvi/README.html

dist: clean_dist
	cd ../ && mkdir -p $(DIST_DIR)
	cd ../ && cp -rf src/brick_game $(DIST_DIR)/brick_game
	cd ../ && cp -rf src/gui $(DIST_DIR)/gui
	cd ../ && cp -rf src/Makefile $(DIST_DIR)/
	cd ../ && tar -czvf archive.tar.gz $(DIST_DIR)
	cd ../ && rm -rf $(DIST_DIR)

dist_unpack:
	cd ../ && tar -xzvf archive.tar.gz


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

rungame: install
	./$(DIR)$(MAIN)

git:
	git add .
	git commit
	git push origin develop

clean:
	rm -rf $(RESULT) $(OBJECTS) $(TEST_OBJECTS) $(TEST_RESULT) gcov_report* *.gc* report/ *.o $(DIR)$(MAIN)

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz

remove_save:
	rm -rf $(DIR)record.save