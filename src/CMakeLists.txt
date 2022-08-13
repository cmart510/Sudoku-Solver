set(INCLUDE_FILES "${SudokuSolver_SOURCE_DIR}/include/sudoku/sudoku.hpp")
set(CUR_LIBRARY Sudoku)

add_library(${CUR_LIBRARY} sudoku.cpp ${INCLUDE_FILES})

target_include_directories(${CUR_LIBRARY} PUBLIC ../include)


source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${INCLUDE_FILES})