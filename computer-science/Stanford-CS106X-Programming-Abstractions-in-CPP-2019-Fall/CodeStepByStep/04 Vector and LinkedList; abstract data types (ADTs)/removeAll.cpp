#include <iostream>
#include <string>
#include <vector>

using namespace std;

void removeAll(vector<string> &vec, const string& str) {
    for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
        if (*iter == str) {
            vec.erase(iter);
            --iter; // Note: iter points to the next item after deleting the item
        }
    }
}

int main() {
    vector<string> v = {"a", "b", "c", "b", "b", "a", "b"};
    removeAll(v, "b");
    
    for (const auto &item : v) {
        cout << item << " ";
    }
    cout << endl;

    return 0;
}
