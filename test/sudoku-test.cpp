#include "gtest/gtest.h"
#include "sudoku/sudoku.hpp"


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

// class SudokuTest : public ::testing::Test {
//     void SetUp() ovveride {

//     }

// };
