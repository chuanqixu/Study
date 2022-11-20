# Universality: U3 and CNOT decomposition

In the explorer universality challenge we introduced the concept of universality but we focused only on a single qubit. Today we will make things more difficult. It has been proven that with the parametric gate [U3](https://docs.pennylane.ai/en/stable/code/api/pennylane.U3.html) and the CNOT gate we can build any operator. In this exercise, we will be asked to work with the following matrix:

$$
\Theta = \begin{pmatrix}
            1 & 1 & 0 & 0 & 0 & 0 & 0 & 0\\
            1 &-1 & 0 & 0 & 0 & 0 & 0 & 0\\
            0 & 0 &-1 &-1 & 0 & 0 & 0 & 0\\
            0 & 0 &-1 & 1 & 0 & 0 & 0 & 0\\
            0 & 0 & 0 & 0 & 1 & 1 & 0 & 0\\
            0 & 0 & 0 & 0 & 1 &-1 & 0 & 0\\
            0 & 0 & 0 & 0 & 0 & 0 & 1 & 1\\
            0 & 0 & 0 & 0 & 0 & 0 & 1 &-1
        \end{pmatrix}
$$


You are asked to create an operator in which only U3 and CNOT gates are used and whose associated matrix is the above. Remember that to check the matrix of an operator you can use the `qml.matrix` function. Let's see an example:

```
def circuit():
    qml.PauliX(wires = 0)
    qml.Hadamard(wires = 1)
    

>> qml.matrix(circuit)()
```

```
array([[ 0.        ,  0.        ,  0.70710678,  0.70710678],
       [ 0.        ,  0.        ,  0.70710678, -0.70710678],
       [ 0.70710678,  0.70710678,  0.        ,  0.        ],
       [ 0.70710678, -0.70710678,  0.        ,  0.        ]])
```

To evaluate your solution, it will be checked that the matrices match and that the gates used are the ones allowed.

Challenge code
--------------

You must complete the `circuit()` function below. Inside the function you simply have to include the necessary operations to generate the requested matrix.

### Inputs and Outputs

In this challenge, we will not have inputs and outputs. The test will be done by checking that the matrix of your circuit coincides with that of the statement. When testing yourself, you can always print the matrix using `qml.matrix(circuit, wire_order=[0,1,2])`.

If your solution matches the correct one within the given tolerance specified in `check` (in this case it's a `1e-05` relative error tolerance), the output will be `"Correct!"`. Otherwise, you will receive a `"Wrong answer"` prompt.

Good luck!