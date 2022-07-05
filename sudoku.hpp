#include <iostream>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <functional>
#include <list>

#define BLANK 10 //greater than 9 to avoid conflict with answers
#define BLANK_STR '-'
#define SIZE 9 //size of Sudoku
#define TILE_SIZE 3 //size of a tile

typedef enum {
    NONE,
    BACKTRACK
} SolverType;

using namespace std;

//Square class represents a square on the board
class square {
    public:
    uint8_t val = BLANK;
    unordered_set<uint8_t> possible;
    square() = default;
    square(const uint8_t val){
        this->val = val;
    }
    square(const uint8_t val, const unordered_set<uint8_t> possible){
        this->val = val;
        this->possible = possible;
    }
    bool removePossible(const uint8_t val) {
        if (possible.erase(val))
            return true;
        return false;
    }
    square operator=(const square s){
        this->val = s.val;
        this->possible = s.possible;
        return *this;
    }
};

//Sudoku class represents a Sudoku board filled with SIZE*SIZE squares
class Sudoku {
    public:
    Sudoku() = default;
    Sudoku(ifstream &in);
    bool isLogical() const { return logical; }
    void print() const;
    bool solve();

    private:
    vector<square> board;
    bool logical = true;

    unsigned int steps = 0;
    SolverType solver_type = NONE;
    
    //Copy of input for comparison after solving
    vector<square> board_initial;
    
    //Algorithms that iterate rows, columns, and tiles and perfrom func on each element
    //Return false if func returns false
    bool eachRow(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func);
    bool eachCol(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func);
    bool eachTile(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func);

    //Check if square is valid
    //Solved squares are the only time potential is BLANK
    bool checkSquare(const uint8_t index, const uint8_t potential = BLANK);

    //Assigns a value to a square and removes it from possible values of related squares
    //Returns true if successful
    bool assignSquare(const uint8_t index, const uint8_t val);

    //Solve function(s)
    bool solveBacktrack();

    //Helper functions
    Sudoku operator=(const Sudoku s);
    void printHelper(const vector<square> board) const;
};