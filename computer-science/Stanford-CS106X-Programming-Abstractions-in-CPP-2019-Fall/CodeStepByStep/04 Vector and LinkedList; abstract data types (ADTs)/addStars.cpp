#include <iostream>
#include <string>
#include <vector>

using namespace std;

void addStars(vector<string> &vec) {
    // Note: because add elemnents into the vector may lead to 
  	//		relocation, which invalidates the iterator, so need
  	//		to reserve the size
    vec.reserve(vec.size()*2 + 1);
    for (auto iter = vec.begin(); iter != vec.end(); iter+=2) {
        vec.insert(iter, "*");
    }
    vec.insert(vec.end(), "*");
}

int main() {
    vector<string> v = {"the", "quick", "brown", "fox"};
    addStars(v);
    
    for (const auto &item : v) {
        cout << item << " ";
    }
    cout << endl;

    return 0;
}
