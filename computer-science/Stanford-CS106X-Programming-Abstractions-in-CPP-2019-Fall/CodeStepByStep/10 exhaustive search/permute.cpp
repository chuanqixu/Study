#include <iostream>
#include <string>

using namespace std;

void permuteHelper(string str, string prev) {
    if (str.size() == 0) {
        cout << prev << endl;
        return;
    }

    for (int i = 0; i < str.size(); ++i) {
        permuteHelper(str.substr(0, i) + str.substr(i+1), prev + str[i]);
    }
}

void permute(string str) {
    permuteHelper(str, "");
}

int main() {
    permute("ABC");

    return 0;
}
