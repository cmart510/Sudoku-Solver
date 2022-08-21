set(TEST_INCLUDE_FILES sudoku-test.cpp)
set(TEST_LINK_LIBRARIES ${PROJECT_NAME} ${PROJECT_NAME}-test)
set(CUR_BINARY ${PROJECT_NAME}-test)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


#Test file inputs
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-inputs/sudoku-test1.txt ${CMAKE_CURRENT_BINARY_DIR}/sudoku-test1.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-inputs/sudoku-test1-solved.txt ${CMAKE_CURRENT_BINARY_DIR}/sudoku-test1-solved.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-inputs/sudoku-test-blank.txt ${CMAKE_CURRENT_BINARY_DIR}/sudoku-test-blank.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-inputs/sudoku-test-invalid.txt ${CMAKE_CURRENT_BINARY_DIR}/sudoku-test-invalid.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-inputs/sudoku-test-complete.txt ${CMAKE_CURRENT_BINARY_DIR}/sudoku-test-complete.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-inputs/sudoku-test-empty.txt ${CMAKE_CURRENT_BINARY_DIR}/sudoku-test-empty.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-inputs/sudoku-test-too-small.txt ${CMAKE_CURRENT_BINARY_DIR}/sudoku-test-too-small.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test-inputs/sudoku-test-too-large.txt ${CMAKE_CURRENT_BINARY_DIR}/sudoku-test-too-large.txt COPYONLY)

add_executable(${CUR_BINARY} ${TEST_INCLUDE_FILES})
target_compile_features(${CUR_BINARY} PRIVATE cxx_std_17)
target_link_libraries(${CUR_BINARY} gtest_main Sudoku)

add_test(NAME ${CUR_BINARY} COMMAND ${CUR_BINARY})

add_custom_command(
  TARGET ${CUR_BINARY}
  COMMENT "Running tests"
  POST_BUIlD_TARGET ${CUR_BINARY}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CUR_BINARY}
)