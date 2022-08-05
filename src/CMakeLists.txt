set(HEADER_LIST "${SudokuSolver_SOURCE_DIR}/include/sudoku/sudoku.hpp")

add_library(Sudoku sudoku.cpp ${HEADER_LIST})

target_include_directories(Sudoku PUBLIC ../include)


source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})