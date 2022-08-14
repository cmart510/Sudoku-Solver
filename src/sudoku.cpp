#include "sudoku/sudoku.hpp"

namespace sudoku {

    Sudoku::Sudoku(std::ifstream &in, const SolverType solver_type) : Sudoku(in){
        this->solver_type = solver_type;
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
                if (grid[i].element <= number_of_element_values){
                    if (!checkSquare(i)){
                        printf("Error: Sudoku default cell %u is not valid\n", i);
                        return;
                    }
                }
                else{ //if the square is blank, fill out possibles
                    for (uint8_t j = 1; j <= number_of_element_values; ++j){
                        if (checkSquare(i, j)){
                            grid[i].possible.insert(j);
                        }
                    }
                    if (grid[i].possible.size() == 0){
                        printf("Error: Sudoku cell %u has no possible inputs\n", i);
                        return;
                    }
                    else if (grid[i].possible.size() == 1){
                        assignSquare(i, *(grid[i].possible.begin()));
                    }
                }
            }
            //If it gets here no errors were found
            logical = true;
        }
    }

    bool Sudoku::eachRow(const uint8_t index, const std::function<bool(uint8_t, std::vector<Square>&)> func){
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

    bool Sudoku::eachCol(const uint8_t index, const std::function<bool(uint8_t, std::vector<Square>&)> func){
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

    bool Sudoku::eachBox(const uint8_t index, const std::function<bool(uint8_t, std::vector<Square>&)> func){
        uint8_t row = index / number_of_element_values;
        uint8_t col = index % number_of_element_values;
        uint8_t tile = (row / box_size)*box_size + (col / box_size);

        uint8_t tile_index = ((row % box_size)*box_size + (col %box_size)); 
        for (uint8_t i = 0; i < number_of_element_values; ++i){
            //Location in current tiles
            uint8_t tileRow = i / box_size;
            uint8_t tileCol = i % box_size;
            //Location in grid
            uint8_t realRow = tileRow + (tile/box_size)*box_size;
            uint8_t realCol = tileCol + (tile%box_size)*box_size;
            //Skip if current square and check function
            if (i != tile_index && !func(realRow*number_of_element_values + realCol, grid)){
                return false;
            }
        }
        return true;
    }

    bool Sudoku::checkSquare(const uint8_t index){
        if (index >= grid_size){
            printf("Error: Index %u is out of bounds\n", index);
            return false;
        }
        //compare index to row, col, and tile
        //Returns true if the compared squares are logical
        auto func = [index] (uint8_t targ_index, std::vector<Square> &grid){
            return grid[index].element != grid[targ_index].element;
        };

        //Check if the square is valid
        return eachRow(index, func) && eachCol(index, func) && eachBox(index, func);
    }

    bool Sudoku::checkSquare(const uint8_t index, const uint8_t potential){
        if (index >= grid_size){
            printf("Error: Index %u is out of bounds\n", index);
            return false;
        }
        if (potential > number_of_element_values){
            printf("Error: Potential %u is out of bounds\n", potential);
            return false;
        }
        //Only change the square if it is blank
        if (grid[index].element != blank_element_value){
            return false;
        }
        //Set test value
        grid[index].element = potential;
        
        bool valid = checkSquare(index);

        //reset the square to blank
        grid[index].element = blank_element_value;

        return valid;
    }

    bool Sudoku::assignSquare(const uint8_t index, const uint8_t val){
        if (index >= grid_size){
            printf("Error: Index %u is out of bounds\n", index);
            return false;
        }
        //Only change the square if it is blank
        if (grid[index].element != blank_element_value){
            return false;
        }
        //Check that val is a valid input at valid index
        if (val > 0 && val <= number_of_element_values){
            grid[index].element = val;

            //We want to to keep possibles when backtracking
            if (solver_type == BACKTRACK){
                return true;
            }

            //Since we are assigning a value, no squares associated can have val as a possible
            auto func = [val] (uint8_t targ_index, std::vector<Square> &grid){
                return grid[targ_index].removePossible(val);
            };

            return eachRow(index, func) && eachCol(index, func) && eachBox(index, func);
        }
        else{
            printf("Error: Invalid input %u at index %u\n", val, index);
            return false;
        }
    }

    bool Sudoku::assignBlankSquare(const uint8_t index){
        if (index >= grid_size){
            printf("Error: Index %u is out of bounds\n", index);
            return false;
        }
        
        grid[index].element = blank_element_value;
        return true;
    }


    Sudoku Sudoku::operator=(const Sudoku s){
        logical = s.logical;
        grid = s.grid;
        steps = s.steps;
        solver_type = s.solver_type;
        return *this;
    }

    void Sudoku::printGrid() const{
        printf("\nInput:");
        printGridStdout(grid, true);
        printf("\nOutput:");
        printGridStdout(grid);
        printf("\nSteps: %u\n", steps);
    }

    //Prints grid to stdout
    void Sudoku::printGridStdout(const std::vector<Square> grid, const bool printGivens) const{
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

            // if (printGivens){
            //     if (grid[i].isGiven()){
            //         printf("%u ", grid[i].element);
            //     }
            //     else{
            //         printf("%c ", blank_input_element_value);
            //     }
            // }

            
            if (grid[i].element == blank_element_value || (printGivens && !grid[i].isGiven())){
                printf("%c ", blank_input_element_value);
            }
            else{
                printf("%u ", grid[i].element);
            }
        }
        printf("|\n~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }

    bool Sudoku::solveBacktrack(){
        solver_type = BACKTRACK;
        ++steps;
        //Find the first blank square, if end of grid is reached, soduko is solved
        uint8_t index;
        auto cur_blank = find_if(grid.begin(), grid.end(), [] (Square s){
            return s.element == blank_element_value;
        });

        if (cur_blank == grid.end()){
            return true;
        }
        else{
            index = cur_blank - grid.begin();
        }

        //Try each possible value
        for (uint8_t val : grid[index].possible){
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
        //Avoid trying to solve illogical grid
        if (!logical){
            return false;
        }
        else{
            return solveBacktrack();
        }
    }
} //End namespace sudoku