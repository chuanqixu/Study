#include <iostream>
#include <string>

using namespace std;

int evaluateMathExpression(const string &exp) {
    auto first_paren_end = exp.find(')'), first_paren_begin = exp.rfind('(', first_paren_end);
    if (first_paren_begin == string::npos) {
        auto operator_pos = exp.find('*');
        if (operator_pos != string::npos) {
            return stoi(exp.substr(0, operator_pos)) * stoi(exp.substr(operator_pos + 1));
        } else {
            operator_pos = exp.find('+');
            if (operator_pos != string::npos) {
                return stoi(exp.substr(0, operator_pos)) + stoi(exp.substr(operator_pos + 1));
            }
            else {
                return stoi(exp);
            }
        }
    } else {
        auto operand = evaluateMathExpression(exp.substr(first_paren_begin + 1, first_paren_end - first_paren_begin - 1));
        if (first_paren_end == exp.length() - 1) {
            return operand;
        } else {
            string new_exp = exp.substr(0, first_paren_begin) + to_string(operand) + exp.substr(first_paren_end + 1);
            return evaluateMathExpression(new_exp);
        }
    }
}

int main() {
    string exp = "(((1+2)*(3+1))+(1*(2+2)))";
    cout << evaluateMathExpression(exp) << endl;

    return 0;
}
