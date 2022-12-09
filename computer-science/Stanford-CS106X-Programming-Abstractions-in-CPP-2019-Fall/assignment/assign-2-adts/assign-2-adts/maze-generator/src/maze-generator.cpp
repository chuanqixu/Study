/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h"
#include <vector>
#include <set>
#include <random>
#include "maze-types.h"
#include "maze-graphics.h"
#include <string>

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



Union_find::size_t cell_to_index(cell c, int dimension) {
    return c.row * dimension + c.col;
}



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



void draw_maze(int dimension) {
    MazeGeneratorView view;
    view.setDimension(dimension);

    vector<wall> walls = generate_walls(dimension), route = get_route(dimension);

    view.drawBorder();
    view.addAllWalls(walls);
    for (const auto &wall : route) {
        view.removeWall(wall);
    }
    cout << "Enter anything to close the maze view.";
    string a;
    cin >> a;
}



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

int main() {
    while (true) {
        int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
        if (dimension == 0) break;
        cout << "This is where I'd animate the construction of a maze of dimension " << dimension << "." << endl;
        draw_maze(dimension);
    }

    return 0;
}
