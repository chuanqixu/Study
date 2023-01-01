/**
 * File: exp.cpp
 * -------------
 * This file implements the exp.h interface.
 */

#include <string>
#include "exp.h"
#include "strlib.h"
#include "error.h"
using namespace std;

/**
 * Implementation notes: Expression
 * --------------------------------
 * The Expression class itself implements only those methods that
 * are not designated as pure virtual.
 */

Expression::Expression() {
   /* Empty */
}

Expression::~Expression() {
   /* Empty */
}

/**
 * Implementation notes: DoubleExp
 * -------------------------------
 * The ConstantExp subclass represents a numeric constant.  The eval
 * method simply returns that value.
 */

DoubleExp::DoubleExp(double value) {
   this->value = value;
}

double DoubleExp::eval(EvaluationContext& /* context */) const {
   return value;
}

string DoubleExp::toString() const {
   return realToString(value);
}

ExpressionType DoubleExp::getType() const {
   return DOUBLE;
}

double DoubleExp::getDoubleValue() const {
   return value;
}

/**
 * Implementation notes: TextStringExp
 * -----------------------------------
 * The IdentifierExp subclass represents a text string constant.  The
 * implementation of eval simply returns 0.0.
 */

TextStringExp::TextStringExp(const string& str) {
    this->str = str;
}

double TextStringExp::eval(EvaluationContext& /* context */) const {
    return 0.0;
}

string TextStringExp::toString() const {
    return str;
}

ExpressionType TextStringExp::getType() const {
    return TEXTSTRING;
}

string TextStringExp::getTextStringValue() const {
    return '"' + str + '"';
}

/**
 * Implementation notes: IdentifierExp
 * -----------------------------------
 * The IdentifierExp subclass represents a variable name.  The
 * implementation of eval looks up that name in the evaluation context.
 */

IdentifierExp::IdentifierExp(const string& name) {
   this->name = name;
}

double IdentifierExp::eval(EvaluationContext& context) const {
   if (!context.isDefined(name)) error(name + " is undefined");
   return context.getValue(name);
}

string IdentifierExp::toString() const {
   return name;
}

ExpressionType IdentifierExp::getType() const {
   return IDENTIFIER;
}

string IdentifierExp::getIdentifierName() const {
   return name;
}

/**
 * Implementation notes: CompoundExp
 * ---------------------------------
 * The implementation of eval for CompoundExp evaluates the left and right
 * subexpressions recursively and then applies the operator.  Assignment is
 * treated as a special case because it does not evaluate the left operand.
 */

CompoundExp::CompoundExp(const string& op, const Expression *lhs, const Expression *rhs) {
   this->op = op;
   this->lhs = lhs;
   this->rhs = rhs;
}

CompoundExp::~CompoundExp() {
   delete lhs;
   delete rhs;
}

double CompoundExp::eval(EvaluationContext & context) const {
   double right = rhs->eval(context);
   double left = lhs->eval(context);
   if (op == "+") return left + right;
   if (op == "-") return left - right;  
   if (op == "*") return left * right;
   if (op == "/") return left / right; // divide by 0.0 gives ±INF
    
   error("Illegal operator in expression.");
   return 0.0;
}

string CompoundExp::toString() const {
   return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

ExpressionType CompoundExp::getType() const {
   return COMPOUND;
}

string CompoundExp::getOperator() const {
   return op;
}

const Expression *CompoundExp::getLHS() const {
   return lhs;
}

const Expression *CompoundExp::getRHS() const {
   return rhs;
}

/**
 * Implementation notes: EvaluationContext
 * ---------------------------------------
 * The methods in the EvaluationContext class simply call the appropriate
 * method on the map used to represent the symbol table.
 */

void EvaluationContext::setValue(const string& var, double value) {
   symbolTable.put(var, value);
}

double EvaluationContext::getValue(const string& var) const {
   return symbolTable.get(var);
}

bool EvaluationContext::isDefined(const string& var) const {
   return symbolTable.containsKey(var);
}








//************************ My Code Below ****************************


/**
 * Implementation notes: FuncExp
 * -----------------------------------
 * The FuncExp subclass represents a function.  The
 * implementation of eval looks up and compute the range in the evaluation context.
 */

FuncExp::FuncExp(string func_name, const range &ran): func_name(func_name), ran(ran) {

    // set the function
    if (func_name == "average")
        func = average;
    else if (func_name == "sum")
        func = sum;
    else if (func_name == "product")
        func = product;
    else if (func_name == "max")
        func = max;
    else if (func_name == "min")
        func = min;
    else if (func_name == "median")
        func = median;
    else if (func_name == "stdev")
        func = stdev;
    else
        error(func_name + " is not supported!");
}

double FuncExp::eval(EvaluationContext& context) const {
    Vector<double> values = range_to_vector(context);
    return func(values);
}

string FuncExp::toString() const {
   return getFuncName() + '(' + getRangeStr() + ')';
}

ExpressionType FuncExp::getType() const {
   return FUNCTION;
}

string FuncExp::getFuncName() const {
    return func_name;
}

string FuncExp::getRangeStr() const {
    return locationToString(ran.startCell) + ':' + locationToString(ran.stopCell);
}

Vector<double> FuncExp::range_to_vector(EvaluationContext &eval_context) const {
    Vector<double> values;
    char start_col = tolower(ran.startCell.col), stop_col = tolower(ran.stopCell.col);
    int start_row = ran.startCell.row, stop_row = ran.stopCell.row;

    if (start_row == stop_row && start_col <= stop_col) {
        for (int i = start_col; i <= stop_col; ++i) {
            string cellname = string(1, i) + integerToString(start_row);
            if (eval_context.isDefined(cellname)) {
                values.add(eval_context.getValue(cellname));
            }
        }
    } else if (start_row < stop_row && start_col == stop_col) {
        for (int i = start_row; i <= stop_row; ++i) {
            string cellname = string(1, start_col) + integerToString(i);
            if (eval_context.isDefined(cellname)) {
                values.add(eval_context.getValue(cellname));
            }
        }
    } else if (start_row < stop_row && start_col < stop_col) {
        for (int i = start_row; i <= stop_row; ++i) {
            for (int j = start_col; j <= stop_col; ++j) {
                string cellname = string(1, j) + integerToString(i);
                if (eval_context.isDefined(cellname)) {
                    values.add(eval_context.getValue(cellname));
                }
            }
        }
    } else {
        error("Input range is invalid!");
    }
    return values;
}
