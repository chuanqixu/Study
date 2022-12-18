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

Stanford implements a library to replace some of the C++ standard libraries used to simplify the development process for their C++ courses. The Stanford C++ Libraries are recommended and widely used in these assignments. [The Stanford C++ Libraries documentation](http://www.martinstepp.com/cppdoc/) needs to be frequently referred to when doing these assignments.


### Assignment Description


I did two kinds of coding assignments for this course. 

The first one is problems on [CodeStepByStep](https://www.codestepbystep.com/), which provides simple coding problems. This assignment is specified in [2017 Fall](http://web.stanford.edu/class/archive/cs/cs106x/cs106x.1182/lectures.shtml).

The second one is the projects. Because non-Stanford students cannot access the course materials of [2017 Fall](http://web.stanford.edu/class/archive/cs/cs106x/cs106x.1182/lectures.shtml), I did assignments posted in [2019 Fall](https://web.stanford.edu/class/cs106x/index.html). There are 7 projects:

1. [The Game of Life](assignment/assign-1-game-of-life/) ([Assignment description](assignment/assign-1-game-of-life/05-Assignment-1-Life.pdf))

    Implement [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#:~:text=The%20Game%20of%20Life%2C%20also,state%2C%20requiring%20no%20further%20input.). In this assignment, you will get familiar with `grid`, a class in `The Stanford C++ Libraries` that is similar to built-in arrays.

    First, need to create a random grid or read from files according to the rules.

    Second, generate the next generation according to the rules of Conway's Game of Life, and display them in a window. Besides, basic user interface interactions need to be implemented, such as the timer or mouse control.

2. [Abstract Data Types (ADTs)](https://en.wikipedia.org/wiki/Abstract_data_type) ([Assignment description](assignment/assign-2-adts/10-Assignment-2-ADTs.pdf))

    This assignment includes three parts:
    1. Word ladder: A simple application of [breadth-first search (BFS)](https://en.wikipedia.org/wiki/Breadth-first_search) to search for intermediate English words that can transform one English word to another by changing only one character each time.
    2. Random maze generator: A simple application of [Kruskal's algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm) that randomly generates a route from the entrance to the exit in a square maze.
    3. Random sentence generator: Given a sentence consisting of nonterminals (sentences that can be expanded by randomly choosing from predefined sentences), expand it to a sentence.

3. The Game of [Boggle](https://en.wikipedia.org/wiki/Boggle) ([Assignment description](assignment/assign-3-boggle/15-Assignment-3-Boggle.pdf))

    This assignment implements a game named [Boggle](https://en.wikipedia.org/wiki/Boggle).

    The main part of this assignment is to use recursion to find all possible English words that are longer than 4 letters in the boggle.

4. Assignment 4: ADTs and [Recursion](https://en.wikipedia.org/wiki/Recursion) ([Assignment description](assignment/assign-4-recursion-and-adts/18-Assignment-4-ADTs-and-Recursion.pdf))

    This assignment implements three recursive problems:

   1. Doctors Without Orders: assign patients to doctors. Each patient has the required hours, and each doctor works for a number of hours. This problem is similar to assigning a series of short line segments to a series of long line segments.
   2. Disaster Preparation: find a set of nodes in a graph where they and their adjacent nodes can cover the whole graph.
   3. Winning the Presidency: dynamic programming but uses recursions instead of loops. Similar to [knapsack problem](https://en.wikipedia.org/wiki/Knapsack_problem).

5. Implementation of [Priority Queue](https://en.wikipedia.org/wiki/Priority_queue) ([Assignment description](assignment/assign-5-pqueue/25-Assignment-5-PQueue.pdf))

    This assignment uses 4 data structures to implement the priority queue:

    1. Unsorted vector
    2. Binary heap
    3. Sorted doubly linked list
    4. [Binomial heap](https://en.wikipedia.org/wiki/Binomial_heap)

6. [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding) ([Assignment description](./29-Assignment-6-Huffman.pdf))

    This assignment implements the Huffman coding.
