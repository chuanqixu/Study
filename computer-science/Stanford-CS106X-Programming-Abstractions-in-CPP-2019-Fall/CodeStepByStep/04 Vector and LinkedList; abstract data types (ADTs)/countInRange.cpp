#include <iostream>
#include <vector>

using namespace std;

int countInRange(vector<int> &vec, int min, int max) {
    int count = 0;
    for (const int num : vec) {
        if (num >= min && num <= max) {
            ++count;
        }
    }
    return count;
}

int main() {
    vector<int> v{28, 1, 17, 4, 41, 9, 59, 8, 31, 30, 25};
    cout << countInRange(v, 10, 30);

    return 0;
}
