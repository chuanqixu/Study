#include <iostream>
#include <queue>

using namespace std;

int main() {
    queue<int> queue;
    for (int i = 1; i <= 6; i++) {
        queue.push(i);
    }
    // {1, 2, 3, 4, 5, 6}
    
    for (int i = 0; i < 3; i++) {
        int value = queue.front();
        queue.pop();
        cout << value << " ";
    }
    
    int size = queue.size();
    cout << "{";
    while(true) {
        int value = queue.front();
        queue.pop();
        if (!queue.empty()){
            cout << value << " ";
        } else {
            cout << value;
            break;
        }
    }
    cout  << "} size " << size << endl;

    // 1 2 3 {4, 5, 6} size 3

    return 0;
}
