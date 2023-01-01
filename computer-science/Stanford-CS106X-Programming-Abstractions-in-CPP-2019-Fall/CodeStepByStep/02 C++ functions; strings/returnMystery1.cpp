#include <iostream>
using namespace std;

int mystery(int b, int c) {
    return c + 2 * b;
}

int main() {
    int a = 4;
    int b = 2;
    int c = 5;

    a = mystery(c, b);
    c = mystery(b, a);
    cout << a << " " << b << " " << c << endl; // 12 2 16
    return 0;
}
