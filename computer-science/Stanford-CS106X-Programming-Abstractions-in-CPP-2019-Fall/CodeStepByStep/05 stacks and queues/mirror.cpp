#include <iostream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

void mirror(queue<string> &q) {
    queue<string>::size_type size = q.size();
    stack<string> s;
    for (int i = 0; i < size; ++i) {
        string value = q.front();
        s.push(value);
        q.pop();
        q.push(value);
    }
    while(!s.empty()) {
        q.push(s.top());
        s.pop();
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
    queue<string> q;
    q.push("a");
    q.push("b");
    q.push("c");

    cout << "Before:" << endl;
    print_queue(q);

    mirror(q);
    cout << "After:" << endl;
    print_queue(q);

    return 0;
}
