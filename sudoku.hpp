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

class square {
    public:
    uint8_t val = BLANK;
    unordered_set<uint8_t> possible;
    square() = default;
    square(uint8_t val){
        this->val = val;
    }
    square(uint8_t val, unordered_set<uint8_t> possible){
        this->val = val;
        this->possible = possible;
    }
    bool removePossible(uint8_t val);
    square operator=(square s){
        this->val = s.val;
        this->possible = s.possible;
        return *this;
    }
};

class Sudoku {
    public:
    bool logical = true;
    vector<square> board;

    Sudoku() = default;
    Sudoku(ifstream &in);
    bool solve();
    void print();


    private:
    SolverType solver_type = NONE;
    
    //Copy of input for comparison after solving
    vector<square> board_initial;
    
    //Algorithms that iterate rows, columns, and tiles and perfrom func on each element
    //Return false if func returns false
    bool eachRow(const uint8_t index, function<bool(uint8_t, vector<square>&)> func);
    bool eachCol(const uint8_t index, function<bool(uint8_t, vector<square>&)> func);
    bool eachTile(const uint8_t index, function<bool(uint8_t, vector<square>&)> func);

    //Combines all the checks into a single function
    //Solved squares are the only time potential is BLANK
    bool checkSquare(uint8_t index, uint8_t potential = BLANK);

    //Checks if the square is valid
    bool checkRow(uint8_t index);
    bool checkCol(uint8_t index);
    bool checkTile(uint8_t index);

    //Assigns a value to a square and removes it from possible values of related squares
    //Returns true if successful
    bool assignSquare(uint8_t index, uint8_t val);


    //Solve function(s)
    bool solve_backtrack();

    Sudoku operator=(Sudoku s);
};