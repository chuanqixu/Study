#include <iostream>
#include "vector.h"

using namespace std;

int main() {
    vector<int> vec;
    for (int i = 1; i <= 8; i++) {
        vec += 10 * i;  //   0   1   2   3   4   5   6   7
    }                   // {10, 20, 30, 40, 50, 60, 70, 80}
    for (int i = 0; i < vec.size(); i++) {
        vec.erase(vec.begin() + i);
    }
    cout << vec << endl; // {20, 40, 60, 80}

    return 0;
}
