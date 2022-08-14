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
        uint8_t element = blank_element_value;
        std::unordered_set<uint8_t> possible;
        bool isGiven() const { return given; }
        bool given = false;
        Square() = default;
        Square(const uint8_t val){
            this->element = val;
        }
        Square(const uint8_t val, const bool given) : Square(val){
            this->given = given;
        }
        Square(const uint8_t val, const std::unordered_set<uint8_t> possible) : Square(val){
            this->possible = possible;
        }
        bool removePossible(const uint8_t val) {
            if (possible.erase(val))
                return true;
            return false;
        }
        Square operator=(const Square s){
            this->element = s.element;
            this->possible = s.possible;
            return *this;
        }
    };

    //Sudoku class represents a Sudoku grid filled with grid_size squares
    class Sudoku {
        public:
        Sudoku() = default;
        Sudoku(std::ifstream &in);
        Sudoku(std::ifstream &in, const SolveMethod solver);
        bool isLogical() const { return logical; }
        SolveMethod getSolverType() const { return solver; }
        bool setSolverType(const SolveMethod solver) { this->solver = solver; return true; }
        std::vector<Square> getBoard() const { return grid; }
        void printGrid() const;
        bool solve();

        Sudoku operator=(const Sudoku s);

        private:
        std::vector<Square> grid;
        bool logical = false;

        unsigned int steps = 0;
        SolveMethod solver = NONE;
        
        //Algorithms that iterate rows, columns, and boxes and perfrom func on each element
        //Return false if any result of func returns false
        bool eachRow(const uint8_t index, const std::function<bool(uint8_t, std::vector<Square>&)> func);
        bool eachCol(const uint8_t index, const std::function<bool(uint8_t, std::vector<Square>&)> func);
        bool eachBox(const uint8_t index, const std::function<bool(uint8_t, std::vector<Square>&)> func);

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
        void printGridStdout(const std::vector<Square> grid, const bool printGivens = false) const;
    };

} //End namespace sudoku

#endif //SUDOKU_HPP