#include <iostream>
#include <stack>

using namespace std;

int main() {
    stack<int> s;
    s.push(7);
    s.push(10);
    cout << s.top() << " ";
    s.pop();
    cout << s.top() << " ";
    s.push(3);
    s.push(5);
    cout << s.top() << " ";
    s.pop();
    cout << s.size() << " ";
    cout << s.top() << " ";
    s.push(8);
    cout << s.top() << " ";
    s.pop();
    cout << s.top() << " ";

    // 10 7 5 2 3 8 3

    return 0;
}
