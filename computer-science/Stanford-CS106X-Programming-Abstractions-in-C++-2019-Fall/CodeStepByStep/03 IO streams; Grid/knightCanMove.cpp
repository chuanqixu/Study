#include <string>
#include "grid.h"

bool knightCanMove(const Grid<std::string> &grid, int r1, int c1, int r2, int c2){
    if (grid.inBounds(r1, c1) && grid.inBounds(r2, c2) && grid[r1][c1]=="knight" && grid[r2][c2].empty())
        if ((abs(r1-r2) == 1 && abs(c1-c2)==2) ||
            (abs(r1-r2) == 2 && abs(c1-c2)==1))
            return true;
    return false;
}

// recursion version
// however, this problem only consider one step
// note: below recursion version is wrong, the grid registered whether
//      the item has been visited needs to be used
// bool knightCanMove(const Grid<std::string> &grid, int r1, int c1, int r2, int c2){
//     if (!grid.inBounds(r1, c1) || !grid.inBounds(r2, c2) || !grid[r2][c2].empty()){
//         return false;
//     }

//     if (r1 == r2 && c1 == c2){
//         return true;
//     }

//     bool result = false;
//     result |= knightCanMove(grid, r1+2, c1+1, r2, c2);
//     result |= knightCanMove(grid, r1+2, c1-1, r2, c2);
//     result |= knightCanMove(grid, r1-2, c1+1, r2, c2);
//     result |= knightCanMove(grid, r1-2, c1-1, r2, c2);
//     result |= knightCanMove(grid, r1+1, c1+2, r2, c2);
//     result |= knightCanMove(grid, r1+1, c1-2, r2, c2);
//     result |= knightCanMove(grid, r1-1, c1+2, r2, c2);
//     result |= knightCanMove(grid, r1-1, c1-2, r2, c2);
//     return result;
// }

int main(){
    Grid<std::string> grid = {
        {"", "", "", "king", "", "", ""},
        {"", "", "", "", "", "", ""},
        {"", "", "", "", "", "", ""},
        {"", "rook", "", "", "", "", ""},
        {"", "", "", "", "", "", ""},
        {"", "", "", "", "", "", ""},
        {"", "", "", "", "", "", ""}
    };

    knightCanMove(grid, 1, 2, 0, 0);
    return 0;
}