#include <iostream>
#include "vector.h"

using namespace std;

int main() {
    vector<int> vec;
    for (int i = 1; i <= 5; i++) {
        vec += 2 * i;         // {2, 4, 6, 8, 10}
    }
    int size = vec.size();
    for (int i = 0; i < size; i++) {
        vec.insert(vec.begin() + i, 42);    // add 42 at index i
    }
    cout << vec << endl; // {42, 42, 42, 42, 42, 2, 4, 6, 8, 10}

    return 0;
}
