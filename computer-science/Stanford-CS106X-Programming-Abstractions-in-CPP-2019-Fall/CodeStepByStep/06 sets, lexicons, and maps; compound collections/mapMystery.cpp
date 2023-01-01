#include <iostream>
#include <map>

using namespace std;

int main() {
    map<string, string> map;
    map["K"] = "Schwarz";
    map["C"] = "Lee";
    map["M"] = "Sahami";
    map["M"] = "Stepp";
    map.erase("Stepp");
    map.erase("K");
    map["J"] = "Cain";
    map.erase("C, Lee");
    
    for (const auto &item : map) {
        cout << item.first << ": " << item.second << "   ";
    }

    return 0;
}
