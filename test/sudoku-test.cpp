#include "gtest/gtest.h"
#include "sudoku/sudoku.hpp"

//Square tests

class SquareTest : public ::testing::Test {
    protected:
    void SetUp() override {
        s1 = square();
        s2 = square(1);
        s3 = square(2, {1, 2, 3});
    }
    square s1;
    square s2;
    square s3;
};

TEST_F(SquareTest, Constructor) {

    EXPECT_EQ(s1.val, BLANK) << "Default constructor value not blank";
    EXPECT_EQ(s1.possible.size(), 0) << "Default constructor possible not empty";
    
    EXPECT_EQ(s2.val, 1) << "Constructor with value (1) does not set value to 1";
    EXPECT_EQ(s2.possible.size(), 0) << "Constructor with value (1) does not set possible to empty";
    
    EXPECT_EQ(s3.val, 2) << "Constructor with value (2, {1, 2, 3}) does not set value to 2";
    EXPECT_EQ(s3.possible.size(), 3) << "Constructor with value (2, {1, 2, 3}) does not set possible to {1, 2, 3}";
}

TEST_F(SquareTest, Assignment) {
    s1 = s2;
    EXPECT_EQ(s1.val, s2.val) << "Assignment operator does not set value";
    EXPECT_EQ(s1.possible.size(), s2.possible.size()) << "Assignment operator does not set possible size correctly";
    EXPECT_EQ(s1.possible, s2.possible) << "Assignment operator does not set possible values correctly";
}

TEST_F(SquareTest, RemovePossible) {
    EXPECT_TRUE(s3.removePossible(1)) << "Remove possible failed to remove 1";
    EXPECT_FALSE(s3.removePossible(4)) << "Remove possible failed to not remove 4";
    EXPECT_EQ(s3.possible.size(), 2) << "Remove possible does not remove 1 from possible";
}

//Sudoku class tests

TEST(SudokuInputTest, emptyFile){
    ifstream input;
    input.open("sudoku-test-empty.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-empty.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_FALSE(s1.isLogical()) << "Empty file logical not FALSE";
    ASSERT_EQ(s1.getBoard().size(), 0) << "Empty file board not empty";
}


TEST(SudokuInputTest, invalidFile){
    ifstream input;
    input.open("sudoku-test-invalid.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-invalid.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_FALSE(s1.isLogical()) << "Invalid file logical not FALSE";
    ASSERT_EQ(s1.getBoard().size(), 0) << "Invalid file board not empty";
}

TEST(SudokuInputTest, toolarge){
    ifstream input;
    input.open("sudoku-test-too-large.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-too-large.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.isLogical()) << "Too large file logical not TRUE";
    ASSERT_EQ(s1.getBoard().size(), SIZE*SIZE) << "Too large file board not size 81";
}

TEST(SudokuInputTest, toosmall){
    ifstream input;
    input.open("sudoku-test-too-small.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-too-small.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_FALSE(s1.isLogical()) << "Too small file logical not FALSE";
    ASSERT_LT(s1.getBoard().size(), SIZE*SIZE) << "Too small file board not smaller than size 81";
}

TEST(SudokuInputTest, blank){
    ifstream input;
    input.open("sudoku-test-blank.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-blank.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.isLogical()) << "Blank file logical not TRUE";
    ASSERT_EQ(s1.getBoard().size(), SIZE*SIZE) << "Blank file board not size 81";
    EXPECT_TRUE(s1.solve()) << "Blank file solve not TRUE";
}

TEST(SudokuInputTest, complete){
    ifstream input;
    input.open("sudoku-test-complete.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-complete.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.isLogical()) << "Complete file logical not TRUE";
    ASSERT_EQ(s1.getBoard().size(), SIZE*SIZE) << "Complete file board not size 81";
    EXPECT_TRUE(s1.solve()) << "Complete file solve not TRUE";
}


//Sudoku class tests with valid input
class SudokuTestValidInput : public ::testing::Test {
    protected:
    void SetUp() override {
        input.open("sudoku-test1.txt");
        ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test1.txt"; 
    }
    ifstream input;
};

TEST_F(SudokuTestValidInput, DefaultConstructor) {
    Sudoku s1;
    EXPECT_FALSE(s1.isLogical()) << "Default constructor logical not FALSE";
    EXPECT_EQ(s1.getSolverType(), NONE) << "Default constructor solver type not NONE";
    ASSERT_EQ(s1.getBoard().size(), 0) << "Default constructor board not empty";
}

TEST_F(SudokuTestValidInput, StreamConstructor){
    Sudoku s1(input);
    EXPECT_TRUE(s1.isLogical()) << "Stream constructor logical not TRUE";
    EXPECT_EQ(s1.getSolverType(), NONE) << "Stream constructor solver type not NONE";
    ASSERT_EQ(s1.getBoard().size(), SIZE*SIZE) << "Stream constructor board not size 81";
}

TEST_F(SudokuTestValidInput, StreamMethodConstructor){
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.isLogical()) << "Stream and Method constructor logical not TRUE";
    EXPECT_EQ(s1.getSolverType(), BACKTRACK) << "Stream and Method constructor solver type not BACKTRACK";
    ASSERT_EQ(s1.getBoard().size(), SIZE*SIZE) << "Stream and Method constructor board not size 81";
}

TEST_F(SudokuTestValidInput, AssignmentOperator){
    Sudoku s1(input, BACKTRACK);
    Sudoku s2 = s1;
    EXPECT_TRUE(s2.isLogical()) << "Assignment operator logical not TRUE";
    EXPECT_EQ(s2.getSolverType(), BACKTRACK) << "Assignment operator solver type not BACKTRACK";
    ASSERT_EQ(s2.getBoard().size(), SIZE*SIZE) << "Assignment operator board not size 81";

    s1 = Sudoku();
    EXPECT_FALSE(s1.isLogical()) << "Assignment operator original logical not FALSE after reset";
    EXPECT_EQ(s1.getSolverType(), NONE) << "Assignment operator original solver type not NONE after reset";
    ASSERT_EQ(s1.getBoard().size(), 0) << "Assignment operator original board not empty after reset";
}

TEST_F(SudokuTestValidInput, Solve){
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.solve()) << "Solve not TRUE";
    EXPECT_TRUE(s1.isLogical()) << "Logical not TRUE after successful solve";
    Sudoku s2;
    EXPECT_FALSE(s2.isLogical()) << "Logical not FALSE on empty board";
    EXPECT_FALSE(s2.solve()) << "Solve not FALSE on empty board";
}

class SudokuTestAccessors : public ::testing::Test {
    protected:
    Sudoku s1;
};

TEST_F(SudokuTestAccessors, SolverType){
    EXPECT_EQ(s1.getSolverType(), NONE) << "getSolverType not NONE";
    EXPECT_TRUE(s1.setSolverType(BACKTRACK));
    EXPECT_EQ(s1.getSolverType(), BACKTRACK) << "getSolverType not BACKTRACK";
}

TEST_F(SudokuTestAccessors, Board){
    vector<square> board = s1.getBoard();
    EXPECT_EQ(board.size(), 0) << "bard not size 0";
    board.push_back(square(11));
    EXPECT_GT(board.size(), s1.getBoard().size()) << "board not larger than original after modification";
}
