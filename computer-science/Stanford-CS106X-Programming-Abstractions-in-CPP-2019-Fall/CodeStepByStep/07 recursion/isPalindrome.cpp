#include <iostream>
#include <string>
#include <cstring>

using namespace std;

bool isPalindrome(const string &str) {
    if (str.length() < 2) {
        return true;
    }
    if (tolower(str[0]) == tolower(str[str.length()-1])) {
        return isPalindrome(str.substr(1, str.length() - 2));
    } else {
        return false;
    }
}

int main() {
    cout << isPalindrome("Madam") << endl;

    return 0;
}
