#include "sudoku/sudoku.hpp"

using namespace std;
using namespace sudoku;

int main(int argc, char *argv[]){

    string filename = "sudoku.txt";
    SolverType solver_type = BACKTRACK;

    for (int i = 0; i < argc; ++i){
        string option = argv[i];
        if (option == "-f"){
            filename = argv[i+1];
        }
        if (option == "-b"){
            solver_type = BACKTRACK;
        }
        if (option == "-r"){
            solver_type = RULES;
        }
        if (option == "-h"){
            printf("Usage: %s [-f filename] [-b] [-r]\n", argv[0]);
            printf("-f filename: specify a file to read in. Default is \"sudoku.txt\"\n");
            printf("-b: use backtracking\n");
            printf("-r: use rules (not implemented yet)\n");
            return 1;
        }
    }

    //read Sudoku
    ifstream input;
    printf("Reading in %s\n", filename.c_str());
    input.open(filename.c_str());
    if (!input.is_open()){
        printf("Error: could not open file %s\n", filename.c_str());
        return 0;
    }
    Sudoku sudoku(input, solver_type);
    
    if (!sudoku.isLogical()){
        printf("Error: Sudoku is not logical\n");
        sudoku.printGrid();
        return 0;
    }
    //solve Sudoku
    if (sudoku.solve()){
        printf("Sudoku solved!\n");
    }
    else{
        printf("Sudoku is not solvable\n");
    }

    sudoku.printGrid();
    return 1;
}