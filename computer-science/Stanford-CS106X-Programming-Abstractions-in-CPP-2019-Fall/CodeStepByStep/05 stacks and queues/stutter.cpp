#include <iostream>
#include <queue>

using namespace std;

void stutter(queue<int> &q) {
    queue<int>::size_type size = q.size();
    for (int i = 0; i < size; ++i) {
        int value = q.front();
        q.pop();
        q.push(value);
        q.push(value);
    }
}

template <typename T>
void print_queue(queue<T> q) {
    while(!q.empty()) {
        cout << q.front() << endl;
        q.pop();
    }
}

int main() {
    queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    cout << "Before:" << endl;
    print_queue(q);

    stutter(q);
    cout << "After:" << endl;
    print_queue(q);

    return 0;
}
