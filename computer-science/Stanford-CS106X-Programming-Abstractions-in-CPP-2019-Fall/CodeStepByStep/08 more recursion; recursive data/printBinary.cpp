#include <iostream>

using namespace std;

void printBinary(int num) {
    if (num < 0) {
        cout << "-";
        // In a general case, need to consider special case -2^n,
        // because 2^n will be overflow.
        printBinary(-num);
    } else if (num < 2) {
        cout << num;
    } else {
        printBinary(num / 2);
        cout << num % 2;
    }
}

int main() {
    printBinary(43);
    cout << endl;
    printBinary(-6);
    cout << endl;

    return 0;
}
