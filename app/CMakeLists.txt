set(CUR_BINARY ${PROJECT_NAME})

add_executable(${CUR_BINARY} sudoku_solver.cpp)
target_compile_features(${CUR_BINARY} PRIVATE cxx_std_17)
target_link_libraries(${CUR_BINARY} Sudoku)