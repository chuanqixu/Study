#include <iostream>
#include <string>
#include <fstream>
#include <set>

using namespace std;

int wordCount(const string &filename) {
    ifstream file_stream(filename);
    if (file_stream.fail()) {
        cout << "File does not exist!";
        return -1;
    }
    set<string> unique_words;
    string word;
    while (file_stream >> word) {
        unique_words.insert(word);
    }
    return unique_words.size();
}

int main() {
    string filename = "wordCount_test";
    cout << "File name: " << filename << endl;
    cout << "The number of unique words: " << wordCount(filename) << endl;

    return 0;
}
