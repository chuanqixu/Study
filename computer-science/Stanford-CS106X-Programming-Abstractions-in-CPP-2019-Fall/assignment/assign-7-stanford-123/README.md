# Introduction

[Assignment description](./32-Assignment-7-Stanford-1-2-3.pdf).

This assignment implements a simple spreadsheet, like Microsoft Excel.

I only implement the version without dependencies.

# Implementation

## SSModel

I chose to use 2 variables to store information of cells:

1. `SparseGrid<Expression *> cells`: this store cells as their expressions so that we can update cells based on their expressions when some of their dependent cells are changed.
2. `EvaluationContext eval_context`: because there is a `Map` that maps cell names to its values, this can be used to store the cell values.

The most important and complicated thing is to implement `setCellFromScanner()`. It needs to set the cell (or update the cell), and update all values in `eval_context` based on dependencies.

## FuncExp

The class is defined as below:

```cpp
/**
 * Subclass: FuncExp
 * -----------------------
 * This subclass represents an function used for the expression.
 */

class FuncExp : public Expression {

    typedef double (*func_t)(const Vector<double> & values);
public:

/**
 * Constructor: FuncExp
 * Usage: Expression *exp = new FuncExp(range);
 * -------------------------------------------------
 * The constructor creates an function expression with the specified range.
 */

   FuncExp(std::string func_name, const range &ran);

/* Prototypes for the virtual methods overridden by this class */

   double eval(EvaluationContext& context) const;
   std::string toString() const;
   ExpressionType getType() const;

/* Prototypes of methods specific to this class */
   std::string getFuncName() const;
   std::string getRangeStr() const;

private:
   std::string func_name;
   func_t func;
   range ran;

   Vector<double> range_to_vector(EvaluationContext &eval_context) const;
};
```

The implementations can be found in [code](./assign-7-stanford-123/src/exp.cpp).
