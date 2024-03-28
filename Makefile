CC            = gcc
OPTIMIZATION  = -O3
STANDARD      = -std=c11
COMPILE_FLAGS = -Wall -Werror -Wextra
LIBS          = -lcheck -lm -lrt -lpthread -lsubunit

MAIN      = src/main.c
VIEW      = src/gui/cli/console_view.c
MODEL     = src/brick_game/tetris/tetris.c
ALL_FILES = $(VIEW) $(MODEL) $(MAIN)

.PHONY: all install open uninstall \
		clean_tests clean_report clean \
		dvi dist test gcov_report

all: install open

install: uninstall
	@echo -------------------- INSTALL --------------------
	mkdir build
	$(CC) $(COMPILE_FLAGS) $(OPTIMIZATION) $(STANDARD) $(ALL_FILES) -o build/tetris -lncurses
	@echo -------------------- INSTALL --------------------

open:
	@echo --------------------- OPEN ----------------------
	build/tetris
	@echo --------------------- OPEN ----------------------

uninstall:
	@echo ------------------- UNINSTALL -------------------
	rm -rf build
	@echo ------------------- UNINSTALL -------------------

clean_tests:
	rm -rf unit_tests

clean_report:
	rm -rf report gcov* tetris.info

clean: uninstall clean_tests clean_report
	@echo --------------------- CLEAN ---------------------
	rm -rf tetris.tar.gz
	@echo --------------------- CLEAN ---------------------

dvi:
	@echo ---------------------- DVI ----------------------
	open readme.html
	@echo ---------------------- DVI ----------------------

dist: clean uninstall
	@echo --------------------- DIST ----------------------
	tar czvf tetris.tar.gz .
	@echo --------------------- DIST ----------------------

test:
	@echo --------------------- TEST ----------------------
	$(CC) $(COMPILE_FLAGS) $(OPTIMIZATION) $(STANDARD) $(MODEL) tests/unit_tests.c -o unit_tests $(LIBS)
	./unit_tests
	@echo --------------------- TEST ----------------------

gcov_report: clean_tests clean_report
	@echo -------------------- REPORT ---------------------
	$(CC) $(FLAGS) --coverage tests/unit_tests.c $(MODEL) $(LIBS) -o gcov_test
	./gcov_test
	lcov -t "tetris" -o tetris.info -c -d .
	genhtml -o report tetris.info
	open ./report/index.html
	@echo -------------------- REPORT ---------------------
