#include <iostream>

using namespace std;

int power(int base, int exp) {
    if (exp < 0) {
        throw exp;
    }
    if (exp == 0) {
        return 1;
    }
    if (exp % 2) {
        return base * power(base, exp - 1);
    } else {
        return power(base*base, exp/2);
    }
}

int main() {
    cout << power(3, 4) << endl;

    return 0;
}
