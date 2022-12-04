# Introduction

[Assignment description](./05-Assignment-1-Life.pdf)

Implement [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#:~:text=The%20Game%20of%20Life%2C%20also,state%2C%20requiring%20no%20further%20input.). In this assignment, you will get familiar with `grid`, a class in `Stanford libcs106 library` that is similar to built-in arrays.

   First, need to create a random grid or read from files according to the rules.

   Second, generate the next generation according to the rules of Conway's Game of Life, and display them in a window. Besides, basic user interface interactions need to be implemented, such as the timer or mouse control.

# Design

In this assignment, the most important part is to control how the new grid representing the next generation is generated from the grid representing the previous generation. I still used the heap to store these two grids. This can be more efficient because we can directly use pointers so that grids do not need to be copied every time when generating the next generation. Also, storing two grids because the transformation of all cells is done simultaneously, and no need to think about how to change the grid in an in-place manner.

# The starting Configuration

## Implementation Details

I used `Grid<int> *start_config()` to both prompt and generate the grid. In this function, prompts will be given to the users to either randomly generate the grid or read from a file.

The grid is created and stored in the heap, and the pointer to the grid will be returned by this function. Remember to delete the resource in the main function after everything is done.

In the assignment description, it is required to provide 2 methods to generate the grid.

1. Randomly generate the grid: this can be done with [Stanford `libcs106` random.h](https://web.stanford.edu/dept/cs_edu/resources/cslib_docs/random.html) modules. [`randomInteger(low, high)`](https://web.stanford.edu/dept/cs_edu/resources/cslib_docs/random.html#Function:randomInteger) is used to randomly specify the number of rows and columns of the grid, and also used to specify ages of cells. [`randomBool()`](https://web.stanford.edu/dept/cs_edu/resources/cslib_docs/random.html#Function:randomBool) is used to specify whether to a cell should be occupied.
2. Read from data files: a number of data files can be found in `res/files`. Because how to specify ages is not mentioned, I still used [`randomInteger(1, kMaxAge)`]((https://web.stanford.edu/dept/cs_edu/resources/cslib_docs/random.html#Function:randomInteger)) for cells that are occupied. One thing that needs to be noticed is the file path in Windows, which will be discussed in the next section.

## File Path in Windows

In Windows, Qt creates a new directory for the built project under the parent directory of the project directory if using the default configure settings. The executable files run in this built directory. The current working directory can be checked with [GetCurrentDirectoryA](https://cplusplus.com/forum/windows/187372/) in header `windows.h`:
```cpp
#include <string>
#include <windows.h>

std::string workingdir()
{
    char buf[256];
    GetCurrentDirectoryA(256, buf);
    return std::string(buf) + '\\';
}
```


Therefore, data files in the source directories are not visible in the current working directory. To access data files, first, go to the parent directory, and then go to the source directory.

For example, to access `"assign-1-game-of-life\res\files\Fish"`, you need to input `"../assign-1-game-of-life/res/files/Fish"`.






# Managing the Grid

In this section, I implemented 3 functions:
1. `bool advance_grid(Grid<int> *&before, Grid<int> *&after)`: which generates the new generation from the grid pointed by before, and changes before to point to this new grid. It returns a bool indicating whether the colony reaches a stable state.
2. `void print_grid(LifeDisplay &display, Grid<int> *grid_ptr)`: display the grid in the display window.
3. `void run_animate(LifeDisplay &display, Grid<int> *grid_ptr)`: actually do the jobs specified by the handout. It continuously generates new generations and displays them in the window according to the user-interface interaction rules specifies in the handout.

## Discussions

1. `advance_grid()`:
   1. I switched two input pointers, so that `before` always points to the grid representing the current generation. The reason is that in `run_animate()`, we can only call `advance_grid()` to get a new generation without any other operations. Because of needing to change the pointer, we should pass by reference.
   2. Actually, the second argument, i.e., the pointer pointing to the grid that stores the intermediate values, is not needed. It can be a local variable in `advance_grid()`. However, this needs to call the constructor and destructor in every step, which is not efficient.
2. `print_grid()`: needs to use functionalities in `life-graphics.h`. Please check the details in this file.
3. `run_animate()`: to implement this function, a little knowledge of event-driven programming is needed. In the handout, it gives an example of how to do the task of checking the timer event and the mouse event. Related functionalities are implemented in `gevent.h`.
    ```cpp
    static void runAnimation(LifeDisplay& display, Grid<int>& board, int ms) {
        GTimer timer(ms);
        timer.start();
        while (true) {
            GEvent event = waitForEvent(TIMER_EVENT + MOUSE_EVENT);
            if (event.getEventClass() == TIMER_EVENT) {
                advanceBoard(display, board);
            } else if (event.getEventType() == MOUSE_PRESSED) {
                break;
            }
        }
        timer.stop();
    }
    ```







# Additional Coding Note

## Checking Input

If using `>>` to read the input into an integer, then if the input is not an integer, the state of `cin` must be clear. Otherwise, it will continuously read the wrong value and set the state bit to fail, leading to an infinite loop.

```cpp
int speed;
cin >> speed;
while (cin.fail() || speed > 4 || speed < 0) {
    cout << "Please input the correct speed!" << endl;
    cin.clear(); // Need this line
    cin >> speed;
}
```

However, if input is passed into a `std::string`, then there is no difference in whether to clear the state bits.
