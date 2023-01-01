#include <iostream>

using namespace std;

int mystery(int n) {
    if (n < 10) {
        return n;
    } else {
        int a = n / 10;
        int b = n % 10;
        return mystery(a + b);
    }
}

int main() {
    cout << mystery(648) << endl; // 9

    return 0;
}
