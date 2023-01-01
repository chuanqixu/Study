#include <iostream>
#include <string>
#include <stack>

using namespace std;

int checkBalance(const string &source) {
    stack<char> s;

    int curr = 0;
    for (const char &c : source) {
        if (c == '(' || c == '{') {
            s.push(c);
        } else if (c == ')' || c == '}') {
            if (s.empty()) {
                return curr;
            } else {
                char top_char = s.top();
                s.pop();
                if ((c == ')'  && top_char != '(') || (c == '}' && top_char != '{')) {
                    return curr;
                }
            }
        }
        ++curr;
    }
    
    if (s.empty()){
        return -1;
    } else {
        return source.length();
    }
}

int main() {
    //            index   0123456789012345678901234567890
    cout << checkBalance("if (a(4) > 9) { foo(a(2)); }") << endl;         // returns -1 because balanced
    cout << checkBalance("for (i=0;i&lt;a(3};i++) { foo{); )") << endl;   // returns 17 because } out of order
    cout << checkBalance("while (true) foo(); }{ ()") << endl;            // returns 20 because } doesn't match any {
    cout << checkBalance("if (x) {") << endl;                             // returns 8 because { is never closed

    return 0;
}
