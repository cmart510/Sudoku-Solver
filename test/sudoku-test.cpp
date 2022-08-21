#include "gtest/gtest.h"
#include "sudoku/sudoku.hpp"
#include <queue>

using namespace std;
using namespace sudoku;

//Square tests

class SquareTest : public ::testing::Test {
    protected:
    void SetUp() override {
        s1 = Square();
        s2 = Square(1, true);
        s3 = Square(2, {2, 3, 7});
    }
    Square s1;
    Square s2;
    Square s3;
};

TEST_F(SquareTest, Constructor) {

    EXPECT_EQ(s1.getElement(), blank_element_value) << "Default constructor value not blank";
    EXPECT_EQ(s1.getPossibles().size(), 0) << "Default constructor possible not empty";
    EXPECT_FALSE(s1.isGiven()) << "Default constructor given not false";
    EXPECT_FALSE(s1.checkPossibles()) << "Check possibles on blank square with no possibles succeeded";
    
    EXPECT_EQ(s2.getElement(), 1) << "Constructor with value (1) does not set value to 1";
    EXPECT_EQ(s2.getPossibles().size(), 0) << "Constructor with value (1) does not set possible to empty";
    EXPECT_TRUE(s2.isGiven()) << "Constructor with value (1, true) does not set given to true";
    EXPECT_TRUE(s2.checkPossibles()) << "Check possibles on valued square failed";
    
    EXPECT_EQ(s3.getElement(), 2) << "Constructor with value (2, {1, 2, 3}) does not set value to 2";
    EXPECT_EQ(s3.getPossibles().size(), 3) << "Constructor with value (2, {1, 2, 3}) does not set possible to {1, 2, 3}";
    EXPECT_FALSE(s3.isGiven()) << "Constructor with value (2, {1, 2, 3}) does not set given to false";
    EXPECT_TRUE(s3.checkPossibles()) << "Check possibles on valued square with possibles failed";
}

TEST_F(SquareTest, SettersAndGetters) {

    //Blank square
    EXPECT_TRUE(s1.isBlank()) << "isBlank() returns false for blank square";
    EXPECT_TRUE(s1.setElement(7)) << "Set blank element to 7 failed";
    EXPECT_EQ(s1.getElement(), 7) << "get element did return 7 after set element to 7";
    EXPECT_FALSE(s1.setElement(0)) << "Set element to to invalud 0 succeeded";
    EXPECT_FALSE(s1.setElement(10)) << "Set element to invalid 10 succeeded";
    s1.setToOriginalValue();
    EXPECT_TRUE(s1.isBlank()) << "Reset element original blank value failed";

    //Valued square
    EXPECT_TRUE(s2.setElement(4)) << "Set element to 4 failed";
    EXPECT_EQ(s2.getElement(), 4) << "Set element to original value failed";
    s2.setToOriginalValue();
    EXPECT_EQ(s2.getElement(), 1) << "Set element to original number value failed";
}

TEST_F(SquareTest, PossiblesTests) {

    std::unordered_set<uint8_t> possibles_compare = {2, 3, 7};

    //Check possibles
    EXPECT_EQ(s3.getPossibles(), possibles_compare) << "Possibles set to {2, 3, 7} failed";
    EXPECT_TRUE(s1.addPossible(1)) << "Add possible 1 failed";
    EXPECT_EQ(s1.getPossibles().size(), 1) << "Possible size not 1 for blank square after adding possible";
    s1.removePossible(2);
    EXPECT_EQ(s1.getPossibles().size(), 1) << "Possible size not 1 after attemopting to remove invalid value";
    EXPECT_TRUE(s1.addPossible(2)) << "Add second possible failed";
    EXPECT_EQ(s1.getPossibles().size(), 2) << "Possible size not 2 for blank square after adding second possible";
    EXPECT_TRUE(s1.checkPossibles()) << "Check possibles on blank square with possibles failed";
    s1.removePossible(1);
    EXPECT_EQ(s1.getPossibles().size(), 1) << "Possible size not 1 for blank square after removing possible";
    EXPECT_TRUE(s1.checkPossibles()) << "Check possibles on blank square with one possible failed";

    //Remove all
    s3.removeAllPossibles();
    EXPECT_EQ(s3.getPossibles().size(), 0) << "RemoveAllPossibles does not remove all possibles";
}

//Sudoku class tests

TEST(SudokuInputTest, emptyFile){
    ifstream input;
    input.open("sudoku-test-empty.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-empty.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_FALSE(s1.isLogical()) << "Empty file logical not FALSE";
    ASSERT_EQ(s1.getGrid().size(), 0) << "Empty file board not empty";
}


TEST(SudokuInputTest, invalidFile){
    ifstream input;
    input.open("sudoku-test-invalid.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-invalid.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_FALSE(s1.isLogical()) << "Invalid file logical not FALSE";
    ASSERT_EQ(s1.getGrid().size(), 0) << "Invalid file board not empty";
}

TEST(SudokuInputTest, toolarge){
    ifstream input;
    input.open("sudoku-test-too-large.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-too-large.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.isLogical()) << "Too large file logical not TRUE";
    ASSERT_EQ(s1.getGrid().size(), grid_size) << "Too large file board not size 81";
}

TEST(SudokuInputTest, toosmall){
    ifstream input;
    input.open("sudoku-test-too-small.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-too-small.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_FALSE(s1.isLogical()) << "Too small file logical not FALSE";
    ASSERT_LT(s1.getGrid().size(), grid_size) << "Too small file board not smaller than size 81";
}

TEST(SudokuInputTest, blank){
    ifstream input;
    input.open("sudoku-test-blank.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-blank.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.isLogical()) << "Blank file logical not TRUE";
    ASSERT_EQ(s1.getGrid().size(), grid_size) << "Blank file board not size 81";
    EXPECT_TRUE(s1.solve()) << "Blank file solve not TRUE";
}

TEST(SudokuInputTest, complete){
    ifstream input;
    input.open("sudoku-test-complete.txt");
    ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test-complete.txt";
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.isLogical()) << "Complete file logical not TRUE";
    ASSERT_EQ(s1.getGrid().size(), grid_size) << "Complete file board not size 81";
    EXPECT_TRUE(s1.solve()) << "Complete file solve not TRUE";
}


//Sudoku class tests with valid input
class SudokuTestConstructors : public ::testing::Test {
    protected:
    void SetUp() override {
        input.open("sudoku-test1.txt");
        ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test1.txt"; 
    }
    ifstream input;
};

TEST_F(SudokuTestConstructors, DefaultConstructor) {
    Sudoku s1;
    EXPECT_FALSE(s1.isLogical()) << "Default constructor logical not FALSE";
    EXPECT_EQ(s1.getSolverType(), NONE) << "Default constructor solver type not NONE";
    ASSERT_EQ(s1.getGrid().size(), 0) << "Default constructor board not empty";
}

TEST_F(SudokuTestConstructors, StreamConstructor){
    Sudoku s1(input);
    EXPECT_TRUE(s1.isLogical()) << "Stream constructor logical not TRUE";
    EXPECT_EQ(s1.getSolverType(), NONE) << "Stream constructor solver type not NONE";
    ASSERT_EQ(s1.getGrid().size(), grid_size) << "Stream constructor board not size 81";
}

TEST_F(SudokuTestConstructors, StreamMethodConstructor){
    Sudoku s1(input, BACKTRACK);
    EXPECT_TRUE(s1.isLogical()) << "Stream and Method constructor logical not TRUE";
    EXPECT_EQ(s1.getSolverType(), BACKTRACK) << "Stream and Method constructor solver type not BACKTRACK";
    ASSERT_EQ(s1.getGrid().size(), grid_size) << "Stream and Method constructor board not size 81";
}

class SudokuTestValidInput : public ::testing::Test {
    protected:
    void SetUp() override {
        input.open("sudoku-test1.txt");
        ASSERT_TRUE(input.is_open()) << "Failed to open sudoku-test1.txt"; 
        s1 = Sudoku(input);
    }
    Sudoku s1;
    ifstream input;
};

TEST_F(SudokuTestValidInput, EachInFuncs){
    auto false_func = [](uint8_t i, std::vector<Square> &grid){ return false; };
    EXPECT_FALSE(s1.eachInRow(4, false_func)) << "EachInRow not false for false function";
    EXPECT_FALSE(s1.eachInCol(4, false_func)) << "EachInCol not false for false function";
    EXPECT_FALSE(s1.eachInBox(4, false_func)) << "EachInBox not false for false function";

    auto true_func = [](uint8_t i, std::vector<Square> &grid){ return true; };
    EXPECT_TRUE(s1.eachInRow(4, true_func)) << "EachInRow not true for true function";
    EXPECT_TRUE(s1.eachInCol(4, true_func)) << "EachInCol not true for true function";
    EXPECT_TRUE(s1.eachInBox(4, true_func)) << "EachInBox not true for true function";

    uint8_t b = blank_element_value;
    queue<uint8_t> row({b, 9, b, 5, 4, 1, 8, b, 7});
    queue<uint8_t> col({b, b, 4, 9, 3, 7, 6, 5, 8});
    queue<uint8_t> box({b, 4, 1, b, 3, b, 8, 7, 9});
    
    auto row_func = [&row](uint8_t i, std::vector<Square> &grid){ row.pop(); return row.front() == grid[i].getElement(); };
    EXPECT_TRUE(s1.eachInRow(31, row_func)) << "EachInRow not true for row function";
    auto col_func = [&col](uint8_t i, std::vector<Square> &grid){ col.pop(); return col.front() == grid[i].getElement(); };
    EXPECT_TRUE(s1.eachInCol(31, col_func)) << "EachInCol not true for col function";
    auto box_func = [&box](uint8_t i, std::vector<Square> &grid){ box.pop(); return box.front() == grid[i].getElement(); };
    EXPECT_TRUE(s1.eachInBox(31, box_func)) << "EachInBox not true for box function";
  
}

TEST_F(SudokuTestValidInput, CheckSquare){

    EXPECT_FALSE(s1.checkSquare(grid_size)) << "Check square with too large index did not return false";
    EXPECT_FALSE(s1.checkSquare(grid_size, 5)) << "Check square with too large index did not return false";
    EXPECT_FALSE(s1.checkSquare(5, number_of_element_values+2)) << "Check square with potential out of range did not return false";
    EXPECT_FALSE(s1.checkSquare(4, 4)) << "Check square with potential conflicting in row does not return false";
    EXPECT_FALSE(s1.checkSquare(4, 8)) << "Check square with potential conflicting in col does not return false";
    EXPECT_FALSE(s1.checkSquare(4, 6)) << "Check square with potential conflicting in box does not return false";
    EXPECT_TRUE(s1.getGrid()[4].isBlank()) << "Failed checkSquares with potential set value and left it";

    EXPECT_TRUE(s1.checkSquare(4, 1)) << "Check square with no conflictions did not return true";
    EXPECT_TRUE(s1.getGrid()[4].isBlank()) << "Success checkSquares with potential set value and left it";
    EXPECT_TRUE(s1.checkSquare(5)) << "Check square with valid value did not return true";

}

TEST_F(SudokuTestValidInput, AssignSquare){

    //Use to test with BACKTRACK on
    Sudoku s2 = s1;

    EXPECT_FALSE(s1.assignSquare(grid_size, 5)) << "Assign square with too large index did not return false";
    EXPECT_FALSE(s1.assignSquare(4, number_of_element_values+2)) << "Assign square with potential out of range did not return false";
    EXPECT_FALSE(s1.assignSquare(5, 4)) << "Assing square to given value succeeded";
    EXPECT_TRUE(s1.getGrid()[4].isBlank()) << "Failed assignSquare with potential set value and left it";
    EXPECT_EQ(s1.getGrid()[5].getElement(), 5) << "Failed assignSquare to given value changed value";

    EXPECT_TRUE(s1.assignSquare(4, 1)) << "Assign square with no conflictions did not return true";

    
    s2.setSolverType(BACKTRACK);

    //Get current number of possibles from related squares
    vector<uint8_t> backtrackPossiblesRelated;

    auto getPossiblesBacktrack = [&backtrackPossiblesRelated](uint8_t i, std::vector<Square> &grid){
        backtrackPossiblesRelated.push_back(grid[i].getPossibles().size()); return true;
    };

    s2.eachInRow(4, getPossiblesBacktrack);
    s2.eachInCol(4, getPossiblesBacktrack);
    s2.eachInBox(4, getPossiblesBacktrack);

    EXPECT_EQ(s2.getGrid()[4].getPossibles().size(), 2) << "Start possibles not correct value";
    EXPECT_TRUE(s2.assignSquare(4, 1)) << "Assign square with no conflictions did not return true in BACKTRACK";
    for (auto i: s2.getGrid()[4].getPossibles()){
        printf("%d ", i);
    }
    EXPECT_EQ(s2.getGrid()[4].getPossibles().size(), 2) << "Assign square removed possibles for set square in BACKTRACK";

    auto curpossible = backtrackPossiblesRelated.begin();

    auto comparePossiblesBacktrack = [&curpossible](uint8_t i, std::vector<Square> &grid){
        EXPECT_EQ(*curpossible, grid[i].getPossibles().size()) << "Possibles not equal for backtrack";
        curpossible++; return true;
    };

    s2.eachInRow(4, comparePossiblesBacktrack);
    s2.eachInCol(4, comparePossiblesBacktrack);
    s2.eachInBox(4, comparePossiblesBacktrack);
}

TEST_F(SudokuTestValidInput, Solve){
    ifstream solution;
    solution.open("sudoku-test1-solved.txt");
    ASSERT_TRUE(solution.is_open()) << "Failed to open sudoku-test-complete.txt";
    Sudoku solved(solution);

    EXPECT_TRUE(s1.solve()) << "Solve not TRUE";
    EXPECT_TRUE(s1.isLogical()) << "Logical not TRUE after successful solve";
    for (int i = 0; i < grid_size; i++){
        EXPECT_EQ(s1.getGrid()[i].getElement(), solved.getGrid()[i].getElement()) << "Solve not correct after successful solve";
    }
    // EXPECT_TRUE(s1.getGrid() == solved.getGrid()) << "Solve not equal to solution";
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
    vector<Square> board = s1.getGrid();
    EXPECT_EQ(board.size(), 0) << "bard not size 0";
    board.push_back(Square(11));
    EXPECT_GT(board.size(), s1.getGrid().size()) << "board not larger than original after modification";
}
