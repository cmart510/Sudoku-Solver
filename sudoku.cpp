#include "sudoku.hpp"

bool square::removePossible(uint8_t val){
    //Note they only need to check square.possible until the possible is greater than the val
    for (auto pos = possible.begin(); pos != possible.end() && *pos <= val; ++pos){
        if (*pos == val){
            possible.erase(pos);
            return true;
        }
    }
    return false;
}
Sudoku::Sudoku(ifstream &in){

    while (in.peek() != EOF && board.size() < SIZE*SIZE){
        char c = in.get();
        //rn limited by using a char.
        if (c > '0' && c <= '9'){
            board.push_back(square(c - '0'));
        }
        else if (c == BLANK_STR){
            board.push_back(square(BLANK));
        }
    }
    if (board.size() != SIZE*SIZE){
        printf("Error: Sudoku file is not the correct size\n");
        logical = false;
        return;
    }
    //Copy the input to the initial board
    board_initial = board;
    //Check if the Sudoku is solvable and fillout possibles
    //Combination of this process saves at least one loop
    // Possibles are not needed for brute force, but do speed it up
    for (uint8_t i = 0; i < SIZE*SIZE; ++i){
        if (board[i].val <= SIZE){
            if (!checkSquare(i)){
                logical = false;
                printf("Error: Sudoku default cell %u is not valid\n", i);
                return;
            }
        }
        else{ //if the square is blank, fill out possibles
            for (uint8_t j = 1; j <= SIZE; ++j){
                if (checkSquare(i, j)){
                    board[i].possible.push_back(j);
                }
            }
            if (board[i].possible.size() == 0){
                logical = false;
                printf("Error: Sudoku cell %u has no possible inputs\n", i);
                return;
            }
            else if (board[i].possible.size() == 1){
                assignSquare(i, board[i].possible[0]);
            }
        }
    }
}


bool Sudoku::eachRow(const uint8_t index, function<bool(uint8_t, vector<square>&)> func){
    uint8_t row = index / SIZE;
    uint8_t col = index % SIZE;

    for (uint8_t i = 0; i < SIZE; ++i){
        if (i != col && !func(row*SIZE + i, board)){
            return false;
        }
    }
    return true;
}

bool Sudoku::eachCol(const uint8_t index, function<bool(uint8_t, vector<square>&)> func){
    uint8_t row = index / SIZE;
    uint8_t col = index % SIZE;

    for (uint8_t i = 0; i < SIZE; ++i){
        if (i != row && !func(i*SIZE + col, board)){
            return false;
        }
    }
    return true;
}

bool Sudoku::eachTile(const uint8_t index, function<bool(uint8_t, vector<square>&)> func){
    uint8_t row = index / SIZE;
    uint8_t col = index % SIZE;
    uint8_t tile = (row / TILE_SIZE)*TILE_SIZE + (col / TILE_SIZE);

    uint8_t tile_index = ((row % TILE_SIZE)*TILE_SIZE + (col %TILE_SIZE)); 
    for (uint8_t i = 0; i < SIZE; ++i){
        uint8_t tileRow = i / TILE_SIZE;
        uint8_t tileCol = i % TILE_SIZE;
        uint8_t realRow = tileRow + (tile/TILE_SIZE)*TILE_SIZE;
        uint8_t realCol = tileCol + (tile%TILE_SIZE)*TILE_SIZE;
        if (i != tile_index && !func(realRow*SIZE + realCol, board)){
            return false;
        }
    }
    return true;
}

bool Sudoku::checkSquare(const uint8_t index, const uint8_t potential){
    uint8_t row = index / SIZE;
    uint8_t col = index % SIZE;
    uint8_t tile = (row / 3)*3 + (col / 3);

    //This should never assign a value to a square that is already solved
    if (potential != BLANK){
        board[index].val = potential;
    }
    
    //compare index to row, col, and tile
    //Returns true if the compared squares are logical
    auto func = [index] (uint8_t targ_index, vector<square> &board) -> bool {
        return board[index].val != board[targ_index].val;
    };

    //Check if the square is valid
    bool valid = eachRow(index, func) && eachCol(index, func) && eachTile(index, func);

    //reset the square to blank
    if (potential != BLANK){
        board[index].val = BLANK;
    }

    return valid;
}

bool Sudoku::assignSquare(uint8_t index, uint8_t val){
    if (val > 0 && val <= SIZE && index < SIZE*SIZE){
        board[index].val = val;
        board[index].possible.clear();

        auto func = [val] (uint8_t targ_index, vector<square> &board) -> bool {
            return board[targ_index].removePossible(val);
        };

        return eachRow(index, func) && eachCol(index, func) && eachTile(index, func);
    }
    else{
        return false;
    }
}


Sudoku Sudoku::operator=(Sudoku s){
    logical = s.logical;
    for (uint8_t i = 0; i < SIZE*SIZE; ++i){
        board[i] = s.board[i];
        board_initial[i] = s.board_initial[i];
    }
    return *this;
}

//Prints board to stdout
static void print_helper(vector<square> board){
    for (uint8_t i = 0; i < board.size(); ++i){
        if (i % SIZE == 0){
            printf("\n");
        }
        if (board[i].val == BLANK){
            printf("%c ", BLANK_STR);
        }
        else{
            printf("%u ", board[i].val);
        }
    }
    printf("\n");
}

void Sudoku::print(){
    print_helper(board_initial);
    print_helper(board);
}