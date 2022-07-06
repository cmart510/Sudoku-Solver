CC=clang++
CFLAGS= -std=c++17

all:
	$(CC) $(CFLAGS) main.cpp sudoku.cpp -o SudokuSolver

clean:
	rm -f SudokuSolver

build: clean all
