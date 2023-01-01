#include <iostream>
#include <vector>

using namespace std;

void diceRollsHelper(int num, vector<int> &chosen) {
    if (num == 0) {
        cout << "{";
        for (auto iter = chosen.cbegin(); iter != chosen.cend() - 1; ++iter) {
            cout << *iter << ", ";
        }
        cout << *(chosen.cend() - 1) << "}" << endl;
        return;
    }

    for (int i = 1; i < 7; ++i) {
        chosen.push_back(i);
        diceRollsHelper(num - 1, chosen);
        chosen.pop_back();
    }
}

void diceRolls(int num) {
    vector<int> chosen;
    diceRollsHelper(num, chosen);
}

int main() {
    diceRolls(3);

    return 0;
}
