#include <iostream>
#include <vector>

using namespace std;

vector<int> intersect(const vector<int> &vec1, const vector<int> &vec2) {
    vector<int> v;
    for (auto iter1 = vec1.cbegin(), iter2 = vec2.cbegin();
        iter1 != vec1.cend() && iter2 != vec2.cend();) {
        if (*iter1 < *iter2) {
            ++iter1;
        } else if (*iter1 == *iter2) {
            v.push_back(*iter1);
            ++iter1;
            ++iter2;
        } else {
            ++iter2;
        }
    }
    return v;
}

int main() {
    vector<int> v1 {1, 4, 8, 9, 11, 15, 17, 28, 41, 59},
                v2 {4, 7, 11, 17, 19, 20, 23, 28, 37, 59, 81};
    
    auto v = intersect(v1, v2);
    for (const auto &item : v) {
        cout << item << " ";
    }
    cout << endl;

    return 0;
}
