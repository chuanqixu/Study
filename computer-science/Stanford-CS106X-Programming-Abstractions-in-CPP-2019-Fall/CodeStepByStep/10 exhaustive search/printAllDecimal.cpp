#include <iostream>
#include <string>

using namespace std;

void printAllDecimalHelper(int digits, string prev) {
    if (digits <= 0) {
        cout << prev << endl;
        return;
    }

    for (int i = 0; i < 10; ++i) {
        printAllDecimalHelper(digits - 1, prev + static_cast<char>(i + '0'));
    }
}

void printAllDecimal(int digits) {
    printAllDecimalHelper(digits, "");
}

int main() {
    printAllDecimal(3);

    return 0;
}
