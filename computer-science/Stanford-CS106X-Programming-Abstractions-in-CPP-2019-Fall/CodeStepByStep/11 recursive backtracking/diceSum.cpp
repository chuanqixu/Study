#include <iostream>
#include <vector>

using namespace std;

void diceSumHelper(int num, int sum, vector<int> &chosen) {
    if (num == 0) {
        if (sum == 0) {
            cout << "{";
            for (auto iter = chosen.cbegin(); iter != chosen.cend() - 1; ++iter) {
                cout << *iter << ", ";
            }
            cout << *(chosen.cend() - 1) << "}" << endl;
        }
        return;
    }

    int max = sum - num < 6 ? sum - num + 1 : 6;
    for (int i = 1; i <= max; ++i) {
        chosen.push_back(i);
        diceSumHelper(num - 1, sum - i, chosen);
        chosen.pop_back();
    }
}

void diceSum(int num, int sum) {
    vector<int> chosen;
    diceSumHelper(num, sum, chosen);
}

int main() {
    diceSum(3, 7);

    return 0;
}
