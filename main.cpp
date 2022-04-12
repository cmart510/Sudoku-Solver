#include "sudoku.hpp"

int main(){
    //read Sudoku
    ifstream input;
    input.open("sudoku.txt");
    Sudoku sudoku(input);
    sudoku.print();
    if (!sudoku.logical){
        cout << "Error: Sudoku is not valid" << endl;
        //sudoku.print();
        return 1;
    }
    //solve Sudoku
    //if (sudoku.solve()){
    //    sudoku.print();
    //}
    //else{
    //    cout << "No solution" << endl;
    //}
    return 1;
}