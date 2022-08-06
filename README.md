# Sudoku-Solver
Personal project that will solve Sudokus using C++ (Work in progress)

Currently only supports backtracking as a solve method. Future plans to add more methods and allow bigger sudokus
For input, it will only read the first 81 valid characters. Any digit (0-9) or '-' is a valid character.
"sudoku.txt" has some example inputs. Recommend copying them into a file one at a time and passing that file into program. Only input param that works is -f.

Compile: 
I have integrated cmake to enable easier cross platform development.

Using defaults
cmake -S <proj root> -B <build dir> 
cmake -S . -B build

Windows:
cmake --build build --config Release
Outputs to build/app/Release/SudokuSolver.exe

Mac:
cmake --build build 
Outputs to build/app/SudokuSolver


To use:
Just running `SudokuSolver` will read in "sudoku.txt" and solve first sudoku
Running `SudokuSolver -f test.txt` will try to read in test.txt

-h for useful tips
-f for custom file input

