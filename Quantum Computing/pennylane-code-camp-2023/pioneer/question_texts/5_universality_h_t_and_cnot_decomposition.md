# Universality: H, T and CNOT decomposition


We already know that universality can be achieved with the U3 gate and the CNOT gate. However, the U3 gate is parameterized so it is really a set of infinite possibilities. This is a problem since in certain situations, for example when delving into the theory behind QEC, we will see that we need to define a **finite** universal set of gates. Fortunately, it has been proven that it is sufficient to have the H, T and CNOT gates to be able to generate any other type of gate.

There are indeed algorithms that allow us to approximate operators from these gates, but it is also important to learn tricks and circuit equivalences that allow us to simplify the process. This will be the objective of this challenge, we will be asked you to construct an operator U that satisfies the following equality:

![](../figs/Uni3.jpeg)

i.e., given an arbitrary state, $\ket\phi \ket\psi$, transform it into $\ket\psi \ket\phi$. (What would be equivalent to a SWAP gate).

To do so, you will only be able to use the H, T and CNOT gates. The solution is obtained exactly, no approximations of any kind are needed.

Note: it is not allowed to use `qml.adjoint(T)`, you can only use H, T and CNOT.

Challenge code
--------------

You simply have to define within the quantum function `U` the gates that form it. Remember that you are only allowed to use H, T and CNOT. You do not have to add anything else to the code.

### Inputs and Outputs

In this challenge we will not have inputs and outputs. The test will be done by checking that the matrix of your circuit coincides with that of the statement. To prototype your code, you can always print the matrix using `qml.matrix(U)`. It should be equal to the matrix associated to the [SWAP gate](https://docs.pennylane.ai/en/stable/code/api/pennylane.SWAP.html).

If your solution matches the correct one within the given tolerance specified in `check` (in this case it's a `1e-05` relative error tolerance), the output will be `"Correct!"`. Otherwise, you will receive a `"Wrong answer"` prompt.

Good luck!