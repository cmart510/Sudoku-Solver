#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <iostream>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <functional>
#include <list>

namespace sudoku{

    //Used to control what solve method to use
    typedef enum {
        NONE,
        RULES,
        BACKTRACK
    } SolveMethod;

    // using namespace std;

    inline constexpr auto blank_element_value = 10;
    inline constexpr auto blank_input_element_value = '-';
    inline constexpr auto number_of_element_values = 9; //How many different values a square can have
    inline constexpr auto grid_size = number_of_element_values*number_of_element_values; //size of grid
    inline constexpr auto box_size = 3; //size of a box 

    //Square class represents a square on the grid
    class Square {
        public:
        bool isGiven() const { return given; }
        Square() = default;
        Square(const uint8_t& val){
            this->element = val;
            this->originalValue = val;
        }
        Square(const uint8_t& val, const bool& given) : Square(val){
            this->given = given;
        }
        Square(const uint8_t& val, const std::unordered_set<uint8_t>& possible) : Square(val){
            this->possibles = possible;
        }
        void setToOriginalValue(){
            this->element = this->originalValue;
        }
        bool setElement(const uint8_t& val){
            //Valid non blank value
            if (val > 0 && val <= number_of_element_values){
                this->element = val;
                return true;
            }
            return false;
        }
        uint8_t getElement() const {
            return this->element;
        }
        bool isBlank() const {
            return this->element == blank_element_value;
        }
        std::unordered_set<uint8_t> getPossibles() const {
            return this->possibles;
        }
        bool addPossible(const uint8_t& val){
            return possibles.insert(val).second;
        }
        bool removePossible(const uint8_t& val) {
            if (possibles.erase(val))
                return true;
            return false;
        }

        //What if checked between possible filling?
        bool checkPossibles() {
            if (element == blank_element_value){
                if (possibles.size() == 0){
                    return false;
                }
                if (possibles.size() == 1){
                    this->element = *possibles.begin();
                }
            }
            return true;
        }
        void removeAllPossibles() {
            possibles.clear();
        }

        private:
        uint8_t element = blank_element_value;
        uint8_t originalValue = blank_element_value;
        std::unordered_set<uint8_t> possibles;
        bool given = false;

    };

    //Sudoku class represents a Sudoku grid filled with grid_size squares
    class Sudoku {
        public:
        Sudoku() = default;
        Sudoku(std::ifstream &in);
        Sudoku(std::ifstream &in, const SolveMethod& solver);
        bool isLogical() const { return logical; }
        SolveMethod getSolverType() const { return solver; }
        bool setSolverType(const SolveMethod& solver) { this->solver = solver; return true; }
        std::vector<Square> getBoard() const { return grid; }
        void printGrid() const;
        bool solve();

        private:
        std::vector<Square> grid;
        bool logical = false;

        unsigned int steps = 0;
        SolveMethod solver = NONE;
        
        //Algorithms that iterate rows, columns, and boxes and perfrom func on each element
        //Return false if any result of func returns false
        bool eachRow(const uint8_t& index, const std::function<bool(uint8_t, std::vector<Square>&)>& func);
        bool eachCol(const uint8_t& index, const std::function<bool(uint8_t, std::vector<Square>&)>& func);
        bool eachBox(const uint8_t& index, const std::function<bool(uint8_t, std::vector<Square>&)>& func);

        //Check if square current value is valid
        bool checkSquare(const uint8_t& index);
        //Check if blank square is valid with a potential value
        bool checkSquare(const uint8_t& index, const uint8_t& potential);

        //Assigns a value to a blank square and removes it from possible values of related squares
        bool assignSquare(const uint8_t& index, const uint8_t& val);

        //Assigns blank to a square
        // bool assignBlankSquare(const uint8_t& index);

        //Solve function(s)
        bool solveBacktrack();

        //Helper functions
        void printGridStdout(const std::vector<Square>& grid, const bool& printGivens = false) const;
    };

} //End namespace sudoku

#endif //SUDOKU_HPP
