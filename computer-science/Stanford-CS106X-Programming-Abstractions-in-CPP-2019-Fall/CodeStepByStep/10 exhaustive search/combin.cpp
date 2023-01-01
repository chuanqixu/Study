#include <iostream>
#include <string>
#include <set>

using namespace std;

void combineHelper(string str, string prev, set<string> &unique_arrangement) {
    if (str.size() == 0) {
        if (unique_arrangement.find(prev) == unique_arrangement.cend()) {
            unique_arrangement.insert(prev);
        }
        return;
    }

    for (int i = 0; i < str.size(); ++i) {
        combineHelper(str.substr(0, i) + str.substr(i+1), prev + str[i], unique_arrangement);
    }
}

void combin(string str) {
    set<string> unique_arrangement;
    combineHelper(str, "", unique_arrangement);
    for (const auto &item : unique_arrangement) {
        cout << item << endl;
    }
}

int main() {
    combin("ABC");

    return 0;
}
