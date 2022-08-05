#include <sudoku/sudoku.hpp>

Sudoku::Sudoku(ifstream &in, const SolverType solver_type) : solver_type(solver_type) {
    constructHelper(in);
}

Sudoku::Sudoku(ifstream &in){
    constructHelper(in);
}

void Sudoku::constructHelper(ifstream &in){
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
    //Right now only used to print at the end of the program
    board_initial = board;

    //Check if the Sudoku is solvable and fill out possibles
    //Possibles are not needed for brute force, but can speed it up
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
                    board[i].possible.insert(j);
                }
            }
            if (board[i].possible.size() == 0){
                logical = false;
                printf("Error: Sudoku cell %u has no possible inputs\n", i);
                return;
            }
            else if (board[i].possible.size() == 1){
                assignSquare(i, *(board[i].possible.begin()));
            }
        }
    }
}

bool Sudoku::eachRow(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func){
    uint8_t row = index / SIZE;
    uint8_t col = index % SIZE;

    for (uint8_t i = 0; i < SIZE; ++i){
        //Skip if current square and check function
        if (i != col && !func(row*SIZE + i, board)){
            return false;
        }
    }
    return true;
}

bool Sudoku::eachCol(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func){
    uint8_t row = index / SIZE;
    uint8_t col = index % SIZE;

    for (uint8_t i = 0; i < SIZE; ++i){
        //Skip if current square and check function
        if (i != row && !func(i*SIZE + col, board)){
            return false;
        }
    }
    return true;
}

bool Sudoku::eachTile(const uint8_t index, const function<bool(uint8_t, vector<square>&)> func){
    uint8_t row = index / SIZE;
    uint8_t col = index % SIZE;
    uint8_t tile = (row / TILE_SIZE)*TILE_SIZE + (col / TILE_SIZE);

    uint8_t tile_index = ((row % TILE_SIZE)*TILE_SIZE + (col %TILE_SIZE)); 
    for (uint8_t i = 0; i < SIZE; ++i){
        //Location in current tiles
        uint8_t tileRow = i / TILE_SIZE;
        uint8_t tileCol = i % TILE_SIZE;
        //Location in board
        uint8_t realRow = tileRow + (tile/TILE_SIZE)*TILE_SIZE;
        uint8_t realCol = tileCol + (tile%TILE_SIZE)*TILE_SIZE;
        //Skip if current square and check function
        if (i != tile_index && !func(realRow*SIZE + realCol, board)){
            return false;
        }
    }
    return true;
}

bool Sudoku::checkSquare(const uint8_t index){
    if (index >= SIZE*SIZE){
        printf("Error: Index %u is out of bounds\n", index);
        return false;
    }
    //compare index to row, col, and tile
    //Returns true if the compared squares are logical
    auto func = [index] (uint8_t targ_index, vector<square> &board){
        return board[index].val != board[targ_index].val;
    };

    //Check if the square is valid
    return eachRow(index, func) && eachCol(index, func) && eachTile(index, func);
}

bool Sudoku::checkSquare(const uint8_t index, const uint8_t potential){
    if (index >= SIZE*SIZE){
        printf("Error: Index %u is out of bounds\n", index);
        return false;
    }
    if (potential > SIZE){
        printf("Error: Potential %u is out of bounds\n", potential);
        return false;
    }
    //Only change the square if it is blank
    if (board[index].val != BLANK){
        return false;
    }
    //Set test value
    board[index].val = potential;
    
    bool valid = checkSquare(index);

    //reset the square to blank
    board[index].val = BLANK;

    return valid;
}

bool Sudoku::assignSquare(const uint8_t index, const uint8_t val){
    if (index >= SIZE*SIZE){
        printf("Error: Index %u is out of bounds\n", index);
        return false;
    }
    //Only change the square if it is blank
    if (board[index].val != BLANK){
        return false;
    }
    //Check that val is a valid input at valid index
    if (val > 0 && val <= SIZE){
        board[index].val = val;

        //We want to to keep possibles when backtracking
        if (solver_type == BACKTRACK){
            return true;
        }

        //Since we are assigning a value, no squares associated can have val as a possible
        auto func = [val] (uint8_t targ_index, vector<square> &board){
            return board[targ_index].removePossible(val);
        };

        return eachRow(index, func) && eachCol(index, func) && eachTile(index, func);
    }
    else{
        printf("Error: Invalid input %u at index %u\n", val, index);
        return false;
    }
}

bool Sudoku::assignBlankSquare(const uint8_t index){
    if (index >= SIZE*SIZE){
        printf("Error: Index %u is out of bounds\n", index);
        return false;
    }
    
    board[index].val = BLANK;
    return true;
}


Sudoku Sudoku::operator=(const Sudoku s){
    logical = s.logical;
    for (uint8_t i = 0; i < SIZE*SIZE; ++i){
        board[i] = s.board[i];
        board_initial[i] = s.board_initial[i];
    }
    return *this;
}

void Sudoku::print() const{
    printf("\nInput:");
    printHelper(board_initial);
    printf("\nOutput:");
    printHelper(board);
    printf("\nSteps: %u\n", steps);
}

//Prints board to stdout
void Sudoku::printHelper(const vector<square> board) const{
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~");
    uint8_t rowSplit = -1; //buffer the first increment
    for (uint8_t i = 0; i < board.size(); ++i){
        if (i % TILE_SIZE == 0 && i != 0){
            printf("| ");
        }
        if (i % SIZE == 0){
            ++rowSplit;
            if (rowSplit == TILE_SIZE){
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~");
                rowSplit = 0;
            }
            printf("\n| ");
        }
        
        if (board[i].val == BLANK){
            printf("%c ", BLANK_STR);
        }
        else{
            printf("%u ", board[i].val);
        }
    }
    printf("|\n~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

bool Sudoku::solveBacktrack(){
    solver_type = BACKTRACK;
    ++steps;
    //Find the first blank square, if end of board is reached, soduko is solved
    uint8_t index;
    auto cur_blank = find_if(board.begin(), board.end(), [] (square s){
        return s.val == BLANK;
    });

    if (cur_blank == board.end()){
        return true;
    }
    else{
        index = cur_blank - board.begin();
    }

    //Try each possible value
    for (uint8_t val : board[index].possible){
        //Check square checks ensures validity before assigning value
        if (checkSquare(index, val) && assignSquare(index, val)){
            if (solveBacktrack()){
                return true;
            }
            else{
                //Revert back to initial state
                assignBlankSquare(index);
            }
        }
    }
    return false;
}

//TODO: Implement new solve methods and utilize solver_type to determine which method to use
bool Sudoku::solve(){
    //Avoid trying to solve illogical board
    if (!logical){
        return false;
    }
    else{
        return solveBacktrack();
    }
}