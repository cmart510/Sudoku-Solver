set(TEST_INCLUDE_FILES sudoku-test.cpp)
set(TEST_LINK_LIBRARIES ${PROJECT_NAME} ${PROJECT_NAME}-test)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}-test ${TEST_INCLUDE_FILES})
target_compile_features(${PROJECT_NAME}-test PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}-test gtest_main Sudoku)

add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-test)