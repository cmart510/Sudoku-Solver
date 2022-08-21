#include "sudoku/sudoku.hpp"

namespace sudoku {

    Sudoku::Sudoku(std::ifstream &in, const SolveMethod& solver) : Sudoku(in){
        this->solver = solver;
    }

    Sudoku::Sudoku(std::ifstream &in){
        if (in.is_open()) {
            while (in.peek() != EOF && grid.size() < grid_size){
                char c = in.get();
                //rn limited by using a char.
                if (c > '0' && c <= '9'){
                    grid.push_back(Square(c - '0', true));
                }
                else if (c == blank_input_element_value){
                    grid.push_back(Square());
                }
            }
            if (grid.size() != grid_size){
                printf("Error: Sudoku file is not the correct size. Got %lu, expected %d.\n", grid.size(), grid_size);
                return;
            }

            //Check if the Sudoku is solvable and fill out possibles
            //Possibles are not needed for brute force, but can speed it up
            for (uint8_t i = 0; i < grid_size; ++i){

                //If blank, fill out possibles
                if(grid[i].isBlank()){
                    for (uint8_t j = 1; j <= number_of_element_values; ++j){
                        if (checkSquare(i, j)){
                            grid[i].addPossible(j);
                        }
                    }
                    //Check if blank square has no possible values
                    if (!grid[i].checkPossibles()){
                        printf("Error: Sudoku cell %u has no possible inputs\n", i);
                        return;
                    }
                }
                else if (!checkSquare(i)){
                    printf("Error: Sudoku default cell %u is not valid\n", i);
                    return;
                }
            }
            //If it gets here no errors were found
            logical = true;
        }
    }

    bool Sudoku::eachRow(const uint8_t& index, const std::function<bool(uint8_t, std::vector<Square>&)>& func){
        uint8_t row = index / number_of_element_values;
        uint8_t col = index % number_of_element_values;

        for (uint8_t i = 0; i < number_of_element_values; ++i){
            //Skip if current square and check function
            if (i != col && !func(row*number_of_element_values + i, grid)){
                return false;
            }
        }
        return true;
    }

    bool Sudoku::eachCol(const uint8_t& index, const std::function<bool(uint8_t, std::vector<Square>&)>& func){
        uint8_t row = index / number_of_element_values;
        uint8_t col = index % number_of_element_values;

        for (uint8_t i = 0; i < number_of_element_values; ++i){
            //Skip if current square and check function
            if (i != row && !func(i*number_of_element_values + col, grid)){
                return false;
            }
        }
        return true;
    }

    bool Sudoku::eachBox(const uint8_t& index, const std::function<bool(uint8_t, std::vector<Square>&)>& func){
        uint8_t row = index / number_of_element_values;
        uint8_t col = index % number_of_element_values;
        uint8_t box = (row / box_size)*box_size + (col / box_size);

        uint8_t boxIndex = ((row % box_size)*box_size + (col %box_size)); 
        for (uint8_t i = 0; i < number_of_element_values; ++i){
            //Location in current box
            uint8_t boxRow = i / box_size;
            uint8_t boxCol = i % box_size;
            //Location in grid
            uint8_t realRow = boxRow + (box/box_size)*box_size;
            uint8_t realCol = boxCol + (box%box_size)*box_size;
            //Skip if current square and check function
            if (i != boxIndex && !func(realRow*number_of_element_values + realCol, grid)){
                return false;
            }
        }
        return true;
    }

    bool Sudoku::checkSquare(const uint8_t& index){
        if (index >= grid_size){
            printf("Error: Index %u is out of bounds\n", index);
            return false;
        }
        //compare index to row, col, and box
        //Returns true if the compared squares are logical
        auto func = [index] (uint8_t targ_index, std::vector<Square> &grid){
            return grid[index].getElement() != grid[targ_index].getElement();
        };

        //Check if the square is valid
        return eachRow(index, func) && eachCol(index, func) && eachBox(index, func);
    }

    bool Sudoku::checkSquare(const uint8_t& index, const uint8_t& potential){
        if (index >= grid_size){
            printf("Error: Index %u is out of bounds\n", index);
            return false;
        }
        if (potential > number_of_element_values){
            printf("Error: Potential %u is out of bounds\n", potential);
            return false;
        }
        
        //Set test value, only success potential is valid and not blank
        if (!grid[index].setElement(potential)){
            return false;
        }
        
        bool valid = checkSquare(index);

        //reset the square to blank
        grid[index].setToOriginalValue();

        return valid;
    }

    bool Sudoku::assignSquare(const uint8_t& index, const uint8_t& val){
        if (index >= grid_size){
            printf("Error: Index %u is out of bounds\n", index);
            return false;
        }
        //Only change the square if it is blank
        if (!grid[index].isBlank()){
            return false;
        }
            
        if (!grid[index].setElement(val)){
            printf("Error: Invalid input %u at index %u\n", val, index);
            return false;
        }
        //We want to to keep possibles when backtracking
        if (solver == BACKTRACK){
            return true;
        }

        //Since we are assigning a value, no squares associated can have val as a possible
        auto func = [val] (uint8_t targ_index, std::vector<Square> &grid){
            return grid[targ_index].removePossible(val);
        };

        return eachRow(index, func) && eachCol(index, func) && eachBox(index, func);
        
    }

    // bool Sudoku::assignBlankSquare(const uint8_t& index){
    //     if (index >= grid_size){
    //         printf("Error: Index %u is out of bounds\n", index);
    //         return false;
    //     }
        
    //     grid[index].element = blank_element_value;
    //     return true;
    // }

    void Sudoku::printGrid() const{
        printf("\nInput:");
        printGridStdout(grid, true);
        printf("\nOutput:");
        printGridStdout(grid);
        printf("\nSteps: %u\n", steps);
    }

    //Prints grid to stdout
    void Sudoku::printGridStdout(const std::vector<Square>& grid, const bool& printGivens) const{
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~");
        uint8_t rowSplit = -1; //buffer the first increment
        for (uint8_t i = 0; i < grid.size(); ++i){
            if (i % box_size == 0 && i != 0){
                printf("| ");
            }
            if (i % number_of_element_values == 0){
                ++rowSplit;
                if (rowSplit == box_size){
                    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~");
                    rowSplit = 0;
                }
                printf("\n| ");
            }
            
            if (grid[i].isBlank() || (printGivens && !grid[i].isGiven())){
                printf("%c ", blank_input_element_value);
            }
            else{
                printf("%u ", grid[i].getElement());
            }
        }
        printf("|\n~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }

    bool Sudoku::solveBacktrack(){
        solver = BACKTRACK;
        ++steps;
        //Find the first blank square, if end of grid is reached, soduko is solved
        uint8_t index;
        auto curBlank = find_if(grid.begin(), grid.end(), [] (Square s){
            return s.isBlank();
        });

        if (curBlank == grid.end()){
            return true;
        }
        else{
            index = curBlank - grid.begin();
        }

        //Try each possible value
        for (uint8_t val : grid[index].getPossibles()){
            //Check square checks ensures validity before assigning value
            if (checkSquare(index, val) && assignSquare(index, val)){
                if (solveBacktrack()){
                    return true;
                }
                else{
                    //Revert back to initial state
                    grid[index].setToOriginalValue();
                }
            }
        }
        return false;
    }

    //TODO: Implement new solve methods and utilize solver to determine which method to use
    bool Sudoku::solve(){
        //Avoid trying to solve illogical grid
        if (!logical){
            return false;
        }
        else{
            return solveBacktrack();
        }
    }
} //End namespace sudoku
