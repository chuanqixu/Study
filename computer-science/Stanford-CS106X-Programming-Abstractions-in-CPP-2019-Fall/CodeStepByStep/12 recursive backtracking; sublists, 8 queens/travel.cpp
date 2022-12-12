#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

void travelHelper(int target_x, int target_y, vector<string> &chosen) {
    if (target_x == 0 && target_y == 0) {
        // There is some problem with the print code on CodeStepByStep
        // It does not allow for in the code, so we cannot use for loop to 
        // print the vector, while `cout << chosen << endl;` will print
        // "{", "}", and "," between each element, which is not the same
        // format as the checking program
        
        // Given that the printing part is not so important for this
        // assignment, I just leave it as it is.
        string chosen_str = accumulate(chosen.cbegin(), chosen.cend(), string(""));
        chosen_str = chosen_str.substr(1);
        cout << chosen_str << endl;

        return;
    }

    if (target_x > 0) {
        chosen.push_back(" E");
        travelHelper(target_x - 1, target_y, chosen);
        chosen.pop_back();
    }
    
    if (target_y > 0) {
        chosen.push_back(" N");
        travelHelper(target_x, target_y - 1, chosen);
        chosen.pop_back();
    }
    
    if (target_x > 0 && target_y > 0) {
        chosen.push_back(" NE");
        travelHelper(target_x - 1, target_y - 1, chosen);
        chosen.pop_back();
    }
}

void travel(int target_x, int target_y) {
    vector<string> chosen;
    travelHelper(target_x, target_y, chosen);
}

int main() {
    travel(2, 1);

    return 0;
}
