cmake_minimum_required(VERSION 3.24)



project(SudokuSolver 
VERSION 0.1
DESCRIPTION "Sudoku Solver"
LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_subdirectory(src)
add_subdirectory(app)




#add_library(Sudoku_libs sudoku.cpp sudoku.hpp)

#add_executable(SudokuSolver sudoku_solver.cpp)

#target_link_libraries(SudokuSolver Sudoku_libs)
