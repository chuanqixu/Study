#include <iostream>
#include <set>

using namespace std;


int main() {
    set<int> set;
    set.insert(74);
    set.insert(12);
    set.insert(74);
    set.insert(74);
    set.insert(43);
    set.erase(74);
    set.erase(999);
    set.erase(43);
    set.insert(32);
    set.insert(12);
    set.insert(9);
    set.insert(999);
    
    for (const auto &item : set) {
        cout << item << " ";
    }
    // {9, 12, 32, 999}

    return 0;
}
