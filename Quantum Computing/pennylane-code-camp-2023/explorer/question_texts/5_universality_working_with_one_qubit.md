# Universality: Working with one qubit

In quantum computing, we have two fundamental structures: states and operators, which we can represent with vectors and matrices respectively. Vectors must have norm 1 (since they determine a sum of probabilities) and matrices must be unitary, i.e. $U^{-1} = U^*$ (to preserve the norm of the vectors). When building a quantum computer, we would like it to be able to generate any $U$ operator we need. However, we cannot physically implement every possible operator, so the goal is to create subsets of gates or operators that are able to generate all the others.

It can be easily proved that in the case of one qubit, we can express any single-qubit unitary $U$ in terms of rotations as follows: $e^{i\phi} R_Z(\gamma) R_X(\beta) R_Z(\alpha)$. This would mean that $R_X$ and $R_Z$ form a universal set. That is why, in this challenge we will be asked to calculate the parameters $\alpha$, $\beta$, and $\gamma$ of a given gate $U$. In particular, we will use a variational method to find these parameters so we will also ask for an error function that tells us how well we are approximating our matrix to $U$.

_Note: Remember that if we write a gate $U$ as $A\cdot B$, it means that $B$ is executed first, and then $A$._

Challenge code
--------------

You must complete the `get_matrix` function below in which you will return the matrix associated with those parameters. Once this is done, a small script will be given to optimize the algorithm looking for the best parameters. In order to do this you must also define the `error` function that determines how well you are approximating your matrix to the target operator.

### Input

As input to this problem, you are given a $2\times2$ complex matrix that you will try to approximate.

### Output

This code will calculate a `list(float)` containing the four parameters ($\phi$, $\alpha$, $\beta$, and $\gamma$) using the `train_parameters` function. Then, the code will output the associated matrix generated from `get_matrix`.

If your solution matches the correct one within the given tolerance specified in `check` (in this case it's a `0.2` absolute error tolerance), the output will be `"Correct!"`. Otherwise, you will receive a `"Wrong answer"` prompt.

Good luck!