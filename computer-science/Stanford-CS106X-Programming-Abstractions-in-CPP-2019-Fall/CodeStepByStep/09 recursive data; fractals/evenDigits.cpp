#include <iostream>

using namespace std;

int evenDigits(int n) {
    if (n == 0) {
        return 0;
    }
    int upper = evenDigits(n/10);
    if (n % 2) {
        return upper;
    } else {
        return upper * 10 + n % 10;
    }
}

int main() {
    cout << evenDigits(8342116) << endl;

    return 0;
}
