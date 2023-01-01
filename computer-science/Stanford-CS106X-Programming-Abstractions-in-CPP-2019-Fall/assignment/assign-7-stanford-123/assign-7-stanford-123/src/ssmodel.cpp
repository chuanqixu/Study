/**
 * File: ssmodel.cpp
 * ------------------
 * This file will contain the implementation of the SSModel class (once you
 * have finished it!
 */
 
#include "ssmodel.h"
#include "exp.h"
using namespace std;



#include <ctype.h>
#include "parser.h"



SSModel::SSModel(int nRows, int nCols, SSView *view): nRows(nRows), nCols(nCols), view_ptr(view) {
    cells = SparseGrid<Expression *>(nRows, nCols);
}

SSModel::~SSModel() {
    // delete cells
    for (int i = 0; i < cells.numRows(); ++i) {
        for (int j = 0; j < cells.numCols(); ++j) {
            delete cells[i][j];
        }
    }
}

bool SSModel::nameIsValid(const string& cellname) const {
    location loc;
    if (!stringToLocation(cellname, loc)) {
        return false;
    }

    int col = tolower(loc.col) - 'a', row = loc.row - 1;

    if (0 <= col && col < nCols && 0 <= row && row < nRows) {
        return true;
    }
    return false;
}

void SSModel::setCellFromScanner(const string& cellname, TokenScanner& scanner) {
    location loc;
    if (!stringToLocation(cellname, loc))
        error("Cellname is not valid!");

    // add expression
    Expression *exp_ptr(parseExp(scanner));
    cells[loc.row - 1][tolower(loc.col) - 'a'] = exp_ptr;

    // update eval_context
    eval_context.setValue(cellname, exp_ptr->eval(eval_context));

    // TODO: update dependencies

    // update display
    update_view(cellname);
}

void SSModel::printCellInformation(const string& cellname) const {
    Expression *exp_ptr = get_cell(cellname);
    location loc;
    stringToLocation(cellname, loc);

    cout << "Cellname: " << cellname << endl;
    cout << "Row: " << loc.row << "\tCol: " << loc.col << endl;
    cout << "Expression Type: " << exp_ptr->getType() << endl;
    cout << "Expression: " << exp_ptr->toString();

    // TODO: dependencies
}

void SSModel::writeToStream(ostream& outfile) const {}

void SSModel::readFromStream(istream& infile) {}

void SSModel::clear() {
    // delete cells
    for (int i = 0; i < cells.numRows(); ++i) {
        for (int j = 0; j < cells.numCols(); ++j) {
            delete cells[i][j];
            view_ptr->displayCell(string(1, i + 'a') + integerToString(j+1), "");
        }
    }

    // create a new cell
    cells = SparseGrid<Expression *>(nRows, nCols);
}

// get the pointer of the cell, or return nullptr if there is no element
Expression *SSModel::get_cell(const std::string &cellname) const {
    location loc;
    if (!stringToLocation(cellname, loc))
        error("Cellname is not valid!");

    return cells[loc.row - 1][tolower(loc.col) - 'a'];
}


bool SSModel::update_eval_context(Expression &exp) {return false;}

// update SSView starting from the given cell
void SSModel::update_view(std::string cellname) {

    Expression *exp_ptr = get_cell(cellname);

    if (exp_ptr->getType() == TEXTSTRING) {
        view_ptr->displayCell(cellname, exp_ptr->toString());
    } else {
        view_ptr->displayCell(cellname, realToString(eval_context.getValue(cellname)));
    }

    // TODO: should use a loop to update all dependent cells
}


