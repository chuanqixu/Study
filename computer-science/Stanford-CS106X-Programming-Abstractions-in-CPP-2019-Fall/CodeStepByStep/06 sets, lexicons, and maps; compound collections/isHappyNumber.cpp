#include <iostream>
#include <set>
#include <cmath>

using namespace std;

bool isHappyNumber(int num) {
    set<int> transversed_nums;
    
    while(true){
        long long sum = 0;
        while (num > 0) {
            sum += pow(num%10, 2);
            num /= 10;
        }

        if (sum == 1) {
            return true;
        }
        
        if (transversed_nums.find(sum) == transversed_nums.cend()) {
            transversed_nums.insert(sum);
        } else {
            return false;
        }

        num = sum;
    }
    
}

int main() {
    cout << "Whether 139 is happy: " << isHappyNumber(139) << endl;
    cout << "Whether 4 is happy: " << isHappyNumber(4) << endl;

    return 0;
}
