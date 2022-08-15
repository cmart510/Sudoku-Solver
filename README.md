
# Sudoku-Solver

Personal project that will solve Sudokus using C++ (Work in progress)

Currently only supports backtracking as a solve method. Future plans to add more solve methods and allow bigger sudokus. 
The application will only read the first valid sudoku from the input file so different puzzles will need to be in seperate files


## Compile

I have integrated cmake to enable easier cross platform development.

Using defaults:
```shell
$ cmake -S <proj root> -B <build dir> 
$ cmake -S . -B build
```

### Mac
Outputs to build/app/SudokuSolver
```shell
$ cmake --build build
```

### Windows
Outputs to build/app/Release/SudokuSolver.exe
```shell
$ cmake --build build --config Release
```

#### Note: 
When building the tests will run automatically you should see the following
```shell
[  PASSED  ] 16 tests.
```
    
## Run Locally

Just running `SudokuSolver` will read in "sudoku.txt" from current directory and solve first sudoku
Running `SudokuSolver -f <path>/test.txt` will try to read in test.txt from `<path>`

#### Current working input params:

`-f <path>` for custom file input.

`-h` for useful tips

### Mac
#### Run SudokuSolver
From project root directory:
```bash
$  ./build/app/SudokuSolver
Reading in sudoku.txt
Sudoku solved!

Input:
~~~~~~~~~~~~~~~~~~~~~~~~~
| 8 3 - | - - 7 | - - - | 
| - - 7 | 9 - - | - 8 - | 
| - - - | - 3 - | 5 - - | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| 5 - - | 6 - - | 1 - - | 
| 2 8 - | - - - | - - 6 | 
| 6 - 1 | - - 2 | - - 9 | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| - - 5 | - 2 - | - - - | 
| - 2 - | - - 4 | 8 - - | 
| - - - | - - - | - 6 1 |
~~~~~~~~~~~~~~~~~~~~~~~~~

Output:
~~~~~~~~~~~~~~~~~~~~~~~~~
| 8 3 9 | 2 5 7 | 6 1 4 | 
| 4 5 7 | 9 6 1 | 3 8 2 | 
| 1 6 2 | 4 3 8 | 5 9 7 | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| 5 9 4 | 6 7 3 | 1 2 8 | 
| 2 8 3 | 1 4 9 | 7 5 6 | 
| 6 7 1 | 5 8 2 | 4 3 9 | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| 7 1 5 | 8 2 6 | 9 4 3 | 
| 9 2 6 | 3 1 4 | 8 7 5 | 
| 3 4 8 | 7 9 5 | 2 6 1 |
~~~~~~~~~~~~~~~~~~~~~~~~~
```

#### Run tests
```shell
$ cd build/test/
$ ./SudokuSolver-test
[==========] Running 16 tests from 4 test suites.
...
[  PASSED  ] 16 tests.
```

### Windows
#### Run SudokuSolver
From project root directory:
```bash
$ ./build/app/Release/SudokuSolver.exe
Reading in sudoku.txt
Sudoku solved!

Input:
~~~~~~~~~~~~~~~~~~~~~~~~~
| 8 3 - | - - 7 | - - - | 
| - - 7 | 9 - - | - 8 - | 
| - - - | - 3 - | 5 - - | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| 5 - - | 6 - - | 1 - - | 
| 2 8 - | - - - | - - 6 | 
| 6 - 1 | - - 2 | - - 9 | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| - - 5 | - 2 - | - - - | 
| - 2 - | - - 4 | 8 - - | 
| - - - | - - - | - 6 1 |
~~~~~~~~~~~~~~~~~~~~~~~~~

Output:
~~~~~~~~~~~~~~~~~~~~~~~~~
| 8 3 9 | 2 5 7 | 6 1 4 | 
| 4 5 7 | 9 6 1 | 3 8 2 | 
| 1 6 2 | 4 3 8 | 5 9 7 | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| 5 9 4 | 6 7 3 | 1 2 8 | 
| 2 8 3 | 1 4 9 | 7 5 6 | 
| 6 7 1 | 5 8 2 | 4 3 9 | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| 7 1 5 | 8 2 6 | 9 4 3 | 
| 9 2 6 | 3 1 4 | 8 7 5 | 
| 3 4 8 | 7 9 5 | 2 6 1 |
~~~~~~~~~~~~~~~~~~~~~~~~~
```

#### Run tests
```shell
$ cd build/test/
$ ./Release/SudokuSolver-test
[==========] Running 16 tests from 4 test suites.
...
[  PASSED  ] 16 tests.
```




## To create your own Sudoku file and try your Sudoku

### Parser requirements
Currently the parser will only take in digits `(1-9)` and `'-'`. Characters are 
read in 1 at a time until 81 valid characters have been read in. All whitespace 
and invalid characters are ignored.

Each digit represents a given value and each '-' represents an unknown value.

### Template:

```shell
~~~~~~~~~~~~~~~~~~~~~~~~~
| - - - | - - - | - - - |
| - - - | - - - | - - - |
| - - - | - - - | - - - |
~~~~~~~~~~~~~~~~~~~~~~~~~
| - - - | - - - | - - - |
| - - - | - - - | - - - |
| - - - | - - - | - - - |
~~~~~~~~~~~~~~~~~~~~~~~~~
| - - - | - - - | - - - |
| - - - | - - - | - - - |
| - - - | - - - | - - - |
~~~~~~~~~~~~~~~~~~~~~~~~~
```

### Examples:

```shell
3 - - - - - 4 - -
- 9 - 7 - 8 - 3 -
5 4 - 6 9 - 2 - 1
9 - - 4 - 1 8 - 7
- - - - 3 - - - -
6 2 4 - - 9 1 5 -
4 - 3 - - 2 - - 5
- - - - - 4 - 8 -
2 - 6 1 8 - 3 - -
```

```shell
~~~~~~~~~~~~~~~~~~~~~~~~~
| 8 3 - | - - 7 | - - - | 
| - - 7 | 9 - - | - 8 - | 
| - - - | - 3 - | 5 - - | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| 5 - - | 6 - - | 1 - - | 
| 2 8 - | - - - | - - 6 | 
| 6 - 1 | - - 2 | - - 9 | 
~~~~~~~~~~~~~~~~~~~~~~~~~
| - - 5 | - 2 - | - - - | 
| - 2 - | - - 4 | 8 - - | 
| - - - | - - - | - 6 1 |
~~~~~~~~~~~~~~~~~~~~~~~~~
```

You can also view sudoku.txt

