# Stanford CS106X Programming Abstractions in C++ (Accelerated)

CS106X is the more advanced of the two courses teaching introductory programming abstractions and algorithms.  CS106X is an alternative to the more sensibly paced CS106B, because some students—self-taught programmers, exceptionally strong CS106A students, and AP Java graduates—prefer a more intense treatment in the company of other aficionados. AP Java and CS106A are all about basic programming practices—expressions, control idioms, decomposition, algorithmic thinking, class design, object orientation, and basic client use of arrays and maps.  CS106X teaches advanced abstraction techniques, worrying first about C++ language mechanics and eventually focusing on topics such as recursion, event-driven programming, C++ lists, sets, and maps, and the implementation techniques used to build custom data structures. 

## Course Website

1. [2017 Fall](http://web.stanford.edu/class/archive/cs/cs106x/cs106x.1182/lectures.shtml): provides lecture recordings, but course materials are not accessible (need Stanford ID). Lecture recordings can be found at [YouTube](https://www.youtube.com/watch?v=pOyyGQU_ErA&list=PLoCMsyE1cvdVmbGH6Jp-9twXPbi5J_IBT) on [Bilibili](https://www.bilibili.com/video/BV1PK411A7S4/?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click).
2. [2019 Fall](https://web.stanford.edu/class/cs106x/index.html): provides course materials such as assignments.

## Coding Assignments

### Important Note Before Doing the Assignments


#### Software

These are the assignments for CS106X 2019 Fall.

Before starting, Qt needs to be installed. I found that the code provided in the assignments is not compatible with newer versions of Qt. In order to work on these assignments, I recommend installing **Qt 5** and configuring with **32-bit** toolchains.

1. Qt modules since Qt 6.0 may not be compatible with these assignments. For example, in the first assignment, [QMediaPlaylist Class](https://doc.qt.io/qt-5/qmediaplaylist.html) is used, which has been removed since Qt 6.0.

    Also, the Qt Multimedia module is not installed by default since Qt 6.0.
    
    Therefore, I recommend installing Qt 5 for this course. For Windows, this can be done with `Qt Maintenance Tool`, which should be attached when downloading Qt.
    The other method (which also works for other operating systems) is to download Qt or Qt Multimedia module only in version 5 from the [archive website](https://download.qt.io/archive/qt/)

2. Use the 32-bit toolchain instead of the 64-bit. The projects cannot be built when I used 64-bit.

#### The Stanford C++ Libraries

Stanford implements a library to replace some of the C++ standard libraries used to simplify the development process for their C++ courses. The Stanford C++ Libraries are recommended and widely used in these assignments. [The Stanford C++ Libraries documentation](http://www.martinstepp.com/cppdoc/) need to be frequently referred to when doing these assignments.


### Assignment Description


I did two kinds of coding assignments for this course. 

The first one is problems on [CodeStepByStep](https://www.codestepbystep.com/), which provides simple coding problems. This assignment is specified in [2017 Fall](http://web.stanford.edu/class/archive/cs/cs106x/cs106x.1182/lectures.shtml).

The second one is the projects. Because non-Stanford students cannot access the course materials of [2017 Fall](http://web.stanford.edu/class/archive/cs/cs106x/cs106x.1182/lectures.shtml), I did assignments posted in [2019 Fall](https://web.stanford.edu/class/cs106x/index.html). There are 7 projects:

1. [The Game of Life](assignment/assign-1-game-of-life/)
   
   [Assignment description](assignment/assign-1-game-of-life/05-Assignment-1-Life.pdf)

    Implement [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#:~:text=The%20Game%20of%20Life%2C%20also,state%2C%20requiring%20no%20further%20input.). In this assignment, you will get familiar with `grid`, a class in `The Stanford C++ Libraries` that is similar to built-in arrays.

   First, need to create a random grid or read from files according to the rules.

   Second, generate the next generation according to the rules of Conway's Game of Life, and display them in a window. Besides, basic user interface interactions need to be implemented, such as the timer or mouse control.
