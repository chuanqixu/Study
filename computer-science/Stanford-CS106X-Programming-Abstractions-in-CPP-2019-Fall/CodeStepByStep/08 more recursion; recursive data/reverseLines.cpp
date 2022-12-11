#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void reverseLines(ifstream &file_stream) {
    string line;
    if (getline(file_stream, line)) {
        reverseLines(file_stream);
        cout << line << endl;
    }
}

int main() {
    ifstream file_stream("poem.txt");
    reverseLines(file_stream);

    return 0;
}
