# Sudoku-Solver
Personal project that will solve Sudokus using C++ (Work in progress)

Currently only supports backtracking as a solve method. Future plans to add more methods. And allow bigger sudokus
For input, it will only read the first 81 valid characters. Any digit (0-9) or '-' is a valid character.
"sudoku.txt" has some example inputs. Recommend copying them into a file one at a time and passing that file into program. Only input param that works is -f.



Compile: I have only tested with gcc and clang++

Windows (gcc):
gcc main.cpp sudoku.cpp -lstdc++ -o SudokuSolver

Mac (clang):
make build


To use:

Just running `SudokuSolver` will read in "sudoku.txt" and solve first sudoku
Running `SudokuSolver -f test.txt` will try to read in test.txt

-h for useful tips
-f for custom file input

