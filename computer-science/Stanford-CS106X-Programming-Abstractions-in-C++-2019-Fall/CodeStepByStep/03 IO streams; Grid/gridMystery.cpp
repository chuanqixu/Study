#include <iostream>
#include "grid.h"

int main(){
    Grid<int> g(4, 3);
    for (int r = 0; r < g.numRows(); r++) {       // {{1, 2, 3},
        for (int c = 0; c < g.numCols(); c++) {   //  {1, 2, 3},
            g[r][c] = c + 1;                      //  {1, 2, 3},
        }                                         //  {1, 2, 3}}
    }
    ​
    for (int c = 0; c < g.numCols(); c++) {
        for (int r = 1; r < g.numRows(); r++) {
            g[r][c] += g[r - 1][c];
        }
    }
    return 0;

    std::cout << g.toString() << std::endl; // {{1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}}
}