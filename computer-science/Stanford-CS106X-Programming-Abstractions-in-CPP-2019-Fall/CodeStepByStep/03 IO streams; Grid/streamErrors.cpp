#include <iostream>
#include <fstream>

using namespace std;

int main(){
    ifstream input;
    input.open("streamErrors-data.txt");

    string name;
    // (1) no member function of std::string named `getline`
    // name = input.getline();      // #1   "Donald Knuth"
    getline(input, name);
    cout << name << endl;

    char gender;
    // (2) istream.get() only gets one character, and a newline character is needed here
    gender = input.get();        // #2   'M'
    cout << gender << endl;
    gender = input.get();

    // (3) // no overload `getline` for istream to int
    // int age;
    string age;
    getline(input, age);
    stringToInteger(age);        // #3   76
    cout << age << endl;

    string jobtitle;
    // (4) `>>` operator only gets one "word" (default delimit by space character)
    // input >> jobtitle;           // #4   "Stanford U."
    getline(input, jobtitle);
    cout << jobtitle << endl;
    
    input.close(); // forget to close the file
}