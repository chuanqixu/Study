#include <iostream>

using namespace std;

int parameterMystery1(int a, int& b, int* c) {
    b++;
    a += *c;
    cout << b << " " << *c << " " << a << " " << c << endl;
    c = &a;
    return a - b;
}

int main() {
    int a = 4;
    int b = 8;
    int c = -3;
    int d;

    d = parameterMystery1(a, b, &c);
    parameterMystery1(c, d, &b);
    parameterMystery1(b, a, &d);
    cout << a << " " << b << " " << c << " " << d << endl;

    return 0;
}
