#include <iostream>
#include <vector>

using namespace std;

void printSubVectorsHelper(vector<int> &vec, int idx, vector<int> &chosen) {
    if (idx == vec.size()) {
        cout << "{";
        if (chosen.empty()) {
            cout << "}" << endl;
            return;
        }
        for (auto iter = chosen.cbegin(); iter != chosen.cend() - 1; ++iter) {
            cout << *iter << ", ";
        }
        cout << *(chosen.cend() - 1) << "}" << endl;

        // Comment above code and use below code to print on CodeStepByStep
        // cout << chosen << endl;

        return;
    }

    int num = vec[idx];

    printSubVectorsHelper(vec, idx + 1, chosen);

    chosen.push_back(num);
    printSubVectorsHelper(vec, idx + 1, chosen);
    chosen.pop_back();
}

void printSubVectors(vector<int> &vec) {
    vector<int> chosen;
    printSubVectorsHelper(vec, 0, chosen);
}

int main() {
    vector<int> vec{1, 2, 3};
    printSubVectors(vec);

    return 0;
}
