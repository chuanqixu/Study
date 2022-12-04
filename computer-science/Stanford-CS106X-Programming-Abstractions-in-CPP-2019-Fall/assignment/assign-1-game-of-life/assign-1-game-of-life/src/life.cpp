/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "strlib.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay


// my include files
#include <string>
#include <fstream>
#include <sstream>
#include "grid.h"
#include "random.h"
#include "gtimer.h"

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}



/**
 * Function: start_config:
 * ------------------------
 * generate the start grid, which is either a random grid or a grid read from a file provided by users.
 * @return the generated grid.
 */
Grid<int> *start_config(){
    cout << "Please select how to generate the grid:" << endl;
    cout << "\t 1) Randomly generate the grid." << endl;
    cout << "\t 2) Read from a data file." << endl;
    cout << "Select (input 1 or 2): ";
    int select=0;
    cin >> select;
    while (cin.fail() || (select != 1 && select != 2)) {
        cout << "Please input a correct selection! " << endl;
        cin.clear();
        cout << "Select (input 1 or 2): ";
        cin >> select;
    }

    Grid<int> *grid_ptr;
    if (select == 1) {
        grid_ptr = new Grid<int>(randomInteger(40, 60), randomInteger(40, 60), 0);
        for (int i = 0; i < grid_ptr->numRows(); ++i){
            for (int j = 0; j < grid_ptr->numCols(); ++j) {
                if (randomBool()) {
                    (*grid_ptr)[i][j] = randomInteger(1, kMaxAge);
                }
            }
        }
    } else if (select == 2) {
        cout << "Please input the file name: ";
        string filename;
        cin >> filename;
        ifstream file(filename);
        while (file.fail()) {
            cout << "File does not exist!" << endl;
            cout << "Please input the file name: ";
            cin >> filename;
            cout << filename << endl;
            file.open(filename);
        }

        string line;
        while(getline(file, line)) {
            if (line[0] != '#') {
                break;
            }
        }
        int num_row, num_col;
        istringstream(line) >> num_row;
        getline(file, line);
        istringstream(line) >> num_col;

        if (file.fail()) {
            cout << "The first two lines should specify the number of rows and columns!" << endl;
            return nullptr;
        }

        grid_ptr = new Grid<int>(num_row, num_col, 0);

        int i = 0;
        while (getline(file, line)) {
            if (line.length() != num_col) {
                cout << "The number of columns in the file does not correspond to the provided data!" << endl;
                return nullptr;
            }
            for (int j = 0; j < num_col; ++j) {
                if (line[j] == 'X') {
                    (*grid_ptr)[i][j] = randomInteger(1, kMaxAge);
                } else if (line[j] != '-') {
                    cout << "Please only specify `-` or `X` in the file" << endl;
                    return nullptr;
                }
            }
            ++i;
        }
        if (i != num_row) {
            cout << "The number of rows in the file does not correspond to the provided data!" << endl;
            return nullptr;
        }
    }

    cout << "Grid is successfully generated!" << endl;
    return grid_ptr;
}


/* Function: advance_grid
 * ----------------------------------
 * Step one generation from grid pointed by before, and stored the grid
 * containing the new generation in the grid pointed by afetr.
 * Also, exchange these two input pointers so that before points to the
 * new grid, while after points to the old grid, which can then be used
 * again to store the new generation when calling this function next time.
 *
 * In addition, a bool is returned to indicate whether the colony reaches
 * a stable state.
 *
 * @return whether the colony is stable.
 */
bool advance_grid(Grid<int> *&before, Grid<int> *&after) {
    bool stable = true;

    // generate the new grid
    for (int i = 0; i < before->numRows(); ++i) {
        for (int j = 0; j < before->numCols(); ++j){
            int num_neighbors = 0;
            for (int c1 = -1; c1 < 2; ++c1){
                for (int c2 = -1; c2 < 2; ++c2) {
                    num_neighbors += before->inBounds(i+c1, j+c2) ? ((*before)[i+c1][j+c2] != 0) : 0;
                }
            }
            num_neighbors -= ((*before)[i][j]) != 0;

            switch(num_neighbors) {
            case 0: case 1:
                (*after)[i][j] = 0;
                if ((*before)[i][j] != 0) {
                    stable = false;
                }
                break;
            case 2:
                (*after)[i][j] = (*before)[i][j] ? ((*before))[i][j] + 1 : 0;
                if ((*before)[i][j] != 0  && (*before)[i][j] < kMaxAge) {
                    stable = false;
                }
                break;
            case 3:
                (*after)[i][j] = (*before)[i][j] + 1;
                if ((*before)[i][j] < kMaxAge) {
                    stable = false;
                }
                break;
            default:
                (*after)[i][j] = 0;
                if ((*before)[i][j] != 0) {
                    stable = false;
                }
            }
        }
    }

    // exchange pointers
    Grid<int> *tmp = before;
    before = after;
    after = tmp;

    return stable;
}



/*
 * Function: print_grid
 * ----------------------------------------
 * print the grid in the display window.
 */
void print_grid(LifeDisplay &display, Grid<int> *grid_ptr) {
    for (int i = 0; i < grid_ptr->numRows(); ++i) {
        for (int j = 0; j < grid_ptr->numCols(); ++j) {
            display.drawCellAt(i, j, (*grid_ptr)[i][j]);
        }
    }
    display.repaint();
}



/*
 * Function: run_animate
 * -----------------------------------------
 * Generate new generations and display them in the
 * window according to the rules specified in the handout.
 */
void run_animate(LifeDisplay &display, Grid<int> *grid_ptr) {
    int speed;
    cout << "We are about to animate the world!" << endl;
    cout << "Before we start, please specify the animate speed." << endl;
    cout << "\t0: Manually hit to advance" << endl;
    cout << "\t1: Slow" << endl;
    cout << "\t2: Medium" << endl;
    cout << "\t3: Fast" << endl;
    cout << "Please specify the speed: ";
    cin >> speed;
    while (cin.fail() || speed > 4 || speed < 0) {
        cout << "Please input the correct speed!" << endl;
        cin.clear();
        cout << "Please specify the speed: ";
        cin >> speed;
    }

    // print the start grid
    print_grid(display, grid_ptr);

    Grid<int> *after = new Grid<int>(grid_ptr->numRows(), grid_ptr->numCols());


    bool stable = false;
    if (speed > 0) {
        GTimer timer(1000/speed);
        timer.start();
        while (!stable) {
            GEvent event = waitForEvent(TIMER_EVENT + MOUSE_EVENT);
            if (event.getEventClass() == TIMER_EVENT) {
                stable = advance_grid(grid_ptr, after);
                print_grid(display, grid_ptr);
            }
            if (event.getEventType() == MOUSE_PRESSED) {
                cout << "Detect that mouse is pressed. Simulation paused!" << endl;
                break;
            }
        }
        timer.stop();
    } else if(speed == 0) {
        while (!stable) {
            GEvent event = waitForEvent(MOUSE_EVENT + KEY_TYPED);
            if (event.getEventType() == MOUSE_PRESSED) {
                stable = advance_grid(grid_ptr, after);
                print_grid(display, grid_ptr);
            } else if (event.getEventType() == KEY_TYPED) {
                string type_word;
                cin >> type_word;
                if (!cin.fail() && type_word == "quit") {
                    cout << "Quit simulation!" << endl;
                    break;
                }
            }
        }
    }

    if (stable) {
        cout << "This colony becomes stable. Simulation end!" << endl;
    } else {
        cout << "Require to quit the simulation!" << endl;
    }

    delete after;
}


/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");

    string if_quit("yes");

    while(toLowerCase(if_quit) == "yes" || toLowerCase(if_quit) == "y"){
        welcome();

        Grid<int> *grid_ptr = start_config();
        if (!grid_ptr){
            cout << "Fail in generate the grid!" << endl;
            return 0;
        }

        display.setDimensions(grid_ptr->numRows(), grid_ptr->numCols());
        run_animate(display, grid_ptr);

        delete grid_ptr;

        cout << "Whether to start another simulation? ('yes' or 'no'): ";
        cin >> if_quit;
        while(cin.fail() || (toLowerCase(if_quit) != "yes" && toLowerCase(if_quit) != "y" && toLowerCase(if_quit) != "no" && toLowerCase(if_quit) != "n")) {
            cout << "Please input the correct command!" << endl;
            cin.clear();
            cout << "Whether to start another simulation? ('yes' or 'no'): ";
            cin >> if_quit;
        }
    }

    return 0;
}
