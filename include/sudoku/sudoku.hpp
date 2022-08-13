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

//Used to control what solve method to use
typedef enum {
    NONE,
    RULES,
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
    Sudoku(ifstream &in, const SolverType solver_type);
    bool isLogical() const { return logical; }
    SolverType getSolverType() const { return solver_type; }
    bool setSolverType(const SolverType solver_type) { this->solver_type = solver_type; return true; }
    vector<square> getBoard() const { return board; }
    void print() const;
    bool solve();

    Sudoku operator=(const Sudoku s);

    private:
    void constructHelper(ifstream &in);
    vector<square> board;
    bool logical = false;

    unsigned int steps = 0;
    SolverType solver_type = NONE;
    
    //Copy of input for comparison after solving
    vector<square> board_initial;
    
    //Algorithms that iterate rows, columns, and tiles and perfrom func on each element
    //Return false if any result of func returns false
    bool eachRow(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func);
    bool eachCol(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func);
    bool eachTile(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func);

    //Check if square current value is valid
    bool checkSquare(const uint8_t index);
    //Check if blank square is valid with a potential value
    bool checkSquare(const uint8_t index, const uint8_t potential);

    //Assigns a value to a blank square and removes it from possible values of related squares
    bool assignSquare(const uint8_t index, const uint8_t val);
    //Assigns blank to a square
    bool assignBlankSquare(const uint8_t index);

    //Solve function(s)
    bool solveBacktrack();

    //Helper functions
    void printHelper(const vector<square> board) const;
};