#include <iostream>
#include <string>

using namespace std;

void printAllBinaryHelper(int digits, string prev) {
    if (digits <= 0) {
        cout << prev << endl;
        return;
    }

    printAllBinaryHelper(digits - 1, prev + "0");
    printAllBinaryHelper(digits - 1, prev + "1");
}

void printAllBinary(int digits) {
    printAllBinaryHelper(digits, "");
}

int main() {
    printAllBinary(3);

    return 0;
}
