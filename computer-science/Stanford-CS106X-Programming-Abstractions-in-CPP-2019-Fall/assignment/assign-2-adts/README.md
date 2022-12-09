# Introduction

[Assignment description](./10-Assignment-2-ADTs.pdf).

This assignment includes three parts:
1. Word ladder
2. Random maze generator
3. Random sentence generator

# Word Ladders

A simple application of [breadth-first search (BFS)](https://en.wikipedia.org/wiki/Breadth-first_search) under the instructions of the handout.

## Implementation

To implement BFS, we need collections below:
1. A `queue` storing the next node to visit.
2. A `set` keeping track of whether the node is visited.
3. A `map` storing the direct previous node for a given node, whose key is one node and the value is its direct parent.

The challenge in this assignment is to "generate" the "nodes" and also the "graph" connect them, and find the condition to add the nodes and edges in the graph. The only difference from a typical BFS is the condition of whether to add a node to visit. All possible next nodes to visit consist of all words that are English-allowed words and have only one difference from the current word. Therefore, we need to change the current word by one character in all its indices. The condition to visit the node in the future is that the new word is English allowed word (and also similar to the typical BFS, we have not visited it before).

In the handout, there is detailed instruction on how to implement BFS. Please check it if you are not sure how to do it.

The code is below:

```cpp
static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;

    queue<string> next; // queue for BSF
    next.push(start);
    set<string> visited; // keep track of whether a word has been visited before
    map<string, string> prev; // keep track of the previous word
    prev.emplace(start, string());

    while (true) {
        string word = next.front();
        next.pop();
        visited.insert(word);

        for (string::size_type i = 0; i < word.size(); ++i) {
            string next_word = word;
            for (char j = 'a'; j <= 'z'; ++j) {
                next_word[i] = j;
                if (next_word == end) {
                    prev.emplace(end, word);

                    // find the ladder
                    vector<string> answer;
                    auto curr = end;
                    while (!curr.empty()) {
                        answer.push_back(curr);
                        curr = prev[curr];
                    }
                    cout << "Found ladder: ";
                    for (auto riter = answer.rbegin(); riter != answer.rend(); ++riter){
                        cout << *riter << " ";
                    }
                    cout << endl << endl;

                    return;
                }
                if (english.contains(next_word) && visited.find(next_word) == visited.end()){
                    next.push(next_word);
                    prev.emplace(next_word, word);
                }
            }
        }
    }
}
```


## Note

Similar to the previous assignment, need to change the file name by default in Windows because the build and running directory is different from the source directory.

In `word-ladder.cpp`, change
```cpp
static const string kEnglishLanguageDatafile = "dictionary.txt";
```
to
```cpp
static const string kEnglishLanguageDatafile = "../word-ladder/res/dictionary.txt";
```





# Generating Mazes

A simple application of [Kruskal's algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm) under the instructions of the handout.

## Implementation

### Union Find

Usually, Kruskal's algorithm is implemented with [disjoint-set data structure (union-find)](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) to find if the selected edge connects two nodes that are in the same set and to combine two sets. Given that the instruction does not mention anything about union find, we should use only `set` and `vector` mentioned in the instruction to implement the union and find functionalities.

The `set` is stored as a tree. Create a `vector` with the same size as the number of cells. For $i$-th item, it stores its parent in the tree. If its parent is itself, it is the head of the tree.

1. Find: to find if nodes $a$ and $b$ are in the same set, recursively check the heads of their trees. If the heads are the same, they are in the same set.
2. Union: to union two sets, first check if the two sets are the same by calling find. If not, directly changing the parent of one head from itself to the head of the other.

Notice that in this implementation, find can usually be improved with compression, and union can usually be improved by appending the lower tree to the higher tree:

1. Find: because when calling find and union, we always need to iterate a tree from some child to the head. The time for this is shorter for a lower tree than a higher tree. Therefore, when implementing find, every time we iterate the tree, we compress the tree. This can be done by changing the parent of the node to search to be the head after we find the head.
2. Union: same reason as above, if we always append the lower tree to the higher tree, we can make the tree height small. This can be done by using a `vector` storing the upper bound of the height viewing each node as the head of its tree. Notice that because it stores the upper bound, we do not need to change this `vector` in the find function when updating its parent.

```cpp
class Union_find {
public:
    typedef vector<int>::size_type size_t;

    Union_find(int n): n(n), parents(vector<int>(n)),
        heights(vector<int>(n)) {
        for (size_t i = 0; i < parents.size(); ++i) {
            parents[i] = i;
        }
    }

    int find(size_t node) {
        if (node >= n) {
            throw "Out of range";
        }

        int head = node;
        while (parents[head] != head) {
            head = parents[head];
        }

        parents[node] = head;
        return head;
    }

    void union_set(size_t a, size_t b) {
        size_t head_a = find(a), head_b = find(b);
        if (head_a == head_b) {
            return;
        }

        if (heights[head_a] < heights[head_b]) {
            parents[head_a] = head_b;
        } else {
            parents[head_b] = head_a;
            if (heights[head_a] == heights[head_b]) {
                ++heights[head_a];
            }
        }
    }

    set<int> get_set(size_t node) {
        set<int> same_set;
        for (size_t i = 0; i < n; ++i) {
            if (find(node) == find(i)) {
                same_set.insert(i);
            }
        }
        return same_set;
    }

private:
    int n;
    vector<int> parents;
    vector<int> heights;
};
```


### Helper Functions

1. In `maze-types.h`, `cell` and `wall` are defined to help simplify the process. A `wall` consists of two `cell`s , and a `cell` consists of its own coordinates.

    ```cpp
    struct cell {
        int row;
        int col;
    };

    struct wall {
        cell one;
        cell two;
    };
    ```

2. Because I use `int` in the union-find function, and actually we use union find to find relations between `cell`s, there should be a function that transforms a `cell` into `int`. This can be simply done by:

    ```cpp
    Union_find::size_t cell_to_index(cell c, int dimension) {
        return c.row * dimension + c.col;
    }
    ```

3. There are both horizontal `wall`s and vertical `wall`s, where the `cell`s have different kinds of coordinates. We need a function to generate them.

    ```cpp
    vector<wall> generate_walls(int dimension) {
        vector<wall> walls;
        for (Union_find::size_t i = 0; i < (dimension - 1) * dimension; ++i) {
            cell one{i/dimension, i%dimension}, two{i/dimension + 1, i%dimension}; // horizontal wall
            walls.push_back(wall{one, two});
            one = {i%dimension, i/dimension}, two = {i%dimension, i/dimension + 1};
            walls.push_back(wall{one, two});
        }
        return walls;
    }
    ```

4. To draw the maze in the window, we need the functionalities of `MazeGeneratorView` defined in `maze-graphics.h`. The function to draw the maze is:

    ```cpp
    static int getMazeDimension(string prompt,
                            int minDimension = 7, int maxDimension = 50) {
        while (true) {
            int response = getInteger(prompt);
            if (response == 0) return response;
            if (response >= minDimension && response <= maxDimension) return response;
            cout << "Please enter a number between "
                << minDimension << " and "
                << maxDimension << ", inclusive." << endl;
        }
    }
    ```


### Random Generate the Route

Because the number of edges of the [Minimum spanning tree](https://en.wikipedia.org/wiki/Minimum_spanning_tree) for an $n$ nodes map is $n-1$, I used a `vector` to store the walls that need to be removed. To get these walls, in every iteration, randomly choose a wall from the `vector` storing all remaining walls. If this wall does connect two sets, add it to the route and union these two sets. In the end, remember to remove it from the remaining wall vector.

```cpp
vector<wall> get_route(int dimension) {
    Union_find uf(dimension * dimension);
    vector<wall> route;
    vector<wall> walls = generate_walls(dimension);
    while (uf.find(0) != uf.find(dimension * dimension - 1)) { // while not connected
        Union_find::size_t idx = rand() % walls.size();
        auto iter = walls.begin();
        for (Union_find::size_t i = 0; i < idx; ++i) {
            ++iter;
        }
        Union_find::size_t idx_one = cell_to_index(iter->one, dimension), idx_two = cell_to_index(iter->two, dimension);
        if (uf.find(idx_one) != uf.find(idx_two)) {
            route.push_back(*iter);
            uf.union_set(idx_one, idx_two);
        }
        walls.erase(iter);
   }

    return route;
}
```

## Note

There are some problems of Stanford C++ Libraries in this assignment. I am not sure whether it is because I used collections from STL. When compiling the code, the error shows `wrong number of template arguments (1, should be 2)` for `/bool operator <(const GenericSet<SetTraits>& set1, const GenericSet<SetTraits>& set2)` in `collections.h` (line 1762-1765).

I commented off this function and it finally successfully compiled and worked.



# Random Sentence Generator

Given a sentence consisting of nonterminals (sentences that can be expanded by randomly choosing from predefined sentences), expand it to a sentence.

## Implementation

1. Use a `map<string, vector<string>>` to store **nonterminals** and their corresponding possible **productions**. However, in this assignment, I used pointers to get rid of the efficiency loss in copy constructors, so the map is stored in the heap. In this case, I change the map type to `map<string, vector<string> *>` so that values are pointers to the vectors, but this is not necessary. The function to generate this map pointer is defined as:

    ```cpp
    map<string, vector<string> *> *generate_nonterminal_map(const string &filename) {
        ifstream file_stream(filename);
        if (!file_stream.is_open()) {
            cout << "Failed to open file!" << endl;
            return nullptr;
        }

        auto *nonterminal_map_ptr = new map<string, vector<string> *>();

        while (true){
            // read the nonterminal
            string line;
            do {
                getline(file_stream, line);
                line = trim(line);
            } while (!file_stream.eof() && line[0] != '<');

            if (file_stream.eof()) {
                break;
            }

            string nonterminal = line;

            // read the number of possible productions
            int num_productions;
            getline(file_stream, line);
            istringstream str_stream(line);
            str_stream >> num_productions;

            // create the vector storing the productions
            (*nonterminal_map_ptr)[nonterminal] = new vector<string>();
            for (int i = 0; i < num_productions; ++i) {
                getline(file_stream, line);
                (*nonterminal_map_ptr)[nonterminal]->push_back(line);
            }
        }

        return nonterminal_map_ptr;
    }
    ```
2. To get the random sentence, we need a function that gets the full productions, i.e., expands all nonterminals and makes the final sentence only contain terminals. Because a nonterminal may still generate a nonterminal, this process can be done recursively. The function is defined as below:

    ```cpp
    string expand(const string sentence, map<string, vector<string> *> *nonterminal_map_ptr) {
        // find the first nonterminal
        auto nonterminal_start = sentence.find('<'), nonterminal_end = sentence.find('>');
        // if there is no nonterminal, directly return the sentence
        if (nonterminal_start == string::npos || nonterminal_end == string::npos) {
            return sentence;
        }

        string expanded_sentence;
        string nonterminal = sentence.substr(nonterminal_start, nonterminal_end - nonterminal_start + 1);

        auto definitions_ptr = (*nonterminal_map_ptr)[nonterminal];
        auto production = (*definitions_ptr)[rand() % definitions_ptr->size()];
        
        // recursively call to expand both the first nonterminal and the following sentence till the end
        expanded_sentence = sentence.substr(0, nonterminal_start) +
                            expand(production, nonterminal_map_ptr) +
                            expand(sentence.substr(nonterminal_end+1), nonterminal_map_ptr);

        return expanded_sentence;
    }
    ```

    This function first randomly selects one production from the **definitions**, and then finds the first nonterminal. Then it recursively calls itself to expand this nonterminal, and also expands the following string till the end.

3. The main function calls the expand on `<start>`.

## Helper Functions

Also remember to delete allocated resources, though for STL objects, it is automatically done.

```cpp
void destroy_nonterminal_map(map<string, vector<string> *> *nonterminal_map_ptr) {
    if (!nonterminal_map_ptr) {
        return;
    }

    for (auto &&item : *nonterminal_map_ptr) {
        delete item.second;
    }
    delete nonterminal_map_ptr;
}
```

## Note

Similar to previous assignments, for Windows and Qt default build directory, paths to files need to be modified.

I modified the global variable `kGrammarsDirectory` from:
```cpp
static const string kGrammarsDirectory = "grammars/";
```
to:
```cpp
static const string kGrammarsDirectory = "../random-sentence-generator/res/grammars/";
```

So that the path issue is solved and only need to input the file names, such as `bond`.
