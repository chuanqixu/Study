/*
 * File: parser.cpp
 * ----------------
 * This file implements the parser.h interface.
 */

#include <iostream>
#include <string>
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "strlib.h"
#include "tokenscanner.h"
using namespace std;



#include <set>

static Expression *readE(TokenScanner& scanner, int prec = 0);
static Expression *readT(TokenScanner& scanner);
static int precedence(const std::string& token);

/**
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner& scanner) {
   Expression *exp = readE(scanner);
   if (scanner.hasMoreTokens()) {
      error("Unexpected token \"" + scanner.nextToken() + "\"");
   }
   return exp;
}

/**
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * The implementation of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each level, the parser reads operators and subexpressions
 * until it finds an operator whose precedence is greater than the prevailing
 * one.  When a higher-precedence operator is found, readE calls itself
 * recursively to read that subexpression as a unit.
 */

Expression *readE(TokenScanner& scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int tprec = precedence(token);
      if (tprec <= prec) break;
      Expression *rhs = readE(scanner, tprec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/**
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */
Expression *readT(TokenScanner& scanner) {
   string token = scanner.nextToken();
   TokenScanner::TokenType type = scanner.getTokenType(token);
   if (type == TokenScanner::WORD){
        // ******************************** My Code ***********************************
        // function
        set<string> funcions{"min", "max", "sum", "product", "average", "median", "stdev"};
        if (funcions.find(toLowerCase(token)) != funcions.end()) {
            string func_name = toLowerCase(token);
            token = scanner.nextToken(); // '('
            token = scanner.nextToken(); // start cell
            location start_cell;
            if (!stringToLocation(token, start_cell))
                error("The name of the start cell is invalid!");
            token = scanner.nextToken(); // ':'
            token = scanner.nextToken(); // stop cell
            location stop_cell;
            if (!stringToLocation(token, stop_cell))
                error("The name of the stop cell is invalid!");
            token = scanner.nextToken(); // ')'

            range ran{start_cell, stop_cell};

            return new FuncExp(func_name, ran);
        }
        // ******************************** My Code ***********************************

        // cellname
        return new IdentifierExp(token);
   }
   if (type == TokenScanner::NUMBER) return new DoubleExp(stringToReal(token));
   if (type == TokenScanner::STRING) return new TextStringExp(token.substr(1, token.length() - 2));
   if (token != "(") error("Unexpected token \"" + token + "\"");
   Expression *exp = readE(scanner, 0);
   if (scanner.nextToken() != ")") {
      error("Unbalanced parentheses");
   }
   return exp;
}

/**
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */
int precedence(const string& token) {
   if (token == "+" || token == "-") return 1;
   if (token == "*" || token == "/") return 2;
   return 0;
}
