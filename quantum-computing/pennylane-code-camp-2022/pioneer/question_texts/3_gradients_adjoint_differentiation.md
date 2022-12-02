# Gradients: Adjoint differentiation

In the last two problems, we dove into the parameter-shift rule. It's an elegant, simple, and _extremely_ useful way to differentiate quantum circuits because of its hardware compatibility. There are other differentiation methods that exist and are very efficient, but are _not_ hardware compatible in general. We'll describe a way to perform one of those methods in this challenge: adjoint differentiation.

Consider a circuit given by a unitary $U(\vec\theta)=\prod_{i=1}^n U_i(\theta_i)$, where $\vec\theta$ are real differentiable parameters, whose output is an expectation value with respect to an observable $A$:

$$
f(\vec\theta) = \bra 0 U^\dagger(\vec\theta) A U(\vec\theta) \ket 0
$$

Let's calculate, say, the derivative of $f$ with respect to the parameter $\theta_i$ from a naive perspective. There are two instances in $f(\vec\theta)$ that involve dependence on $\theta_i$: one from $U_i(\theta_i)$ and one from $U_i^\dagger(\theta_i)$. Thus, we will have to use the product rule as follows.

$$
\frac{\partial}{\partial\theta_i} f(\vec\theta) = \bra 0 \left( \prod_{j=1}^n \frac{\partial}{\partial\theta_i} U^\dagger_j(\theta_j) \right) A U(\vec\theta) \ket 0 \\
+ \bra 0 U^\dagger(\vec\theta) A \left( \prod_{j=1}^n \frac{\partial}{\partial\theta_i} U_j(\theta_j) \right)  \ket 0 \\
= \bra 0 U_1^\dagger \cdots \frac{\partial}{\partial\theta_i} U^\dagger_i \cdots U^\dagger_n A U(\vec\theta) \ket 0 \\
+ \bra 0 U^\dagger(\vec\theta) A U_n \cdots \frac{\partial}{\partial\theta_i} U_i \cdots U_1 \ket 0
$$

Note that we are assuming that each parameterized gate is parameterized by only _one_ parameter, and we've eliminated the $\theta_i$ dependence in each gate $U_i(\theta_i)$ for legibility.

One way to evaluate this expression, albeit a very inefficient way, is to simulate four quantum circuits that all return `qml.state()` with some crucial assumptions:

1.  Each gate $U_i$ must be of the form $U(\theta) = \exp(ic\theta G)$, where $c$ is a real number (constant) and $G$ is unitary ($G$ is often called the "generator" of $U$).
2.  The observable $A$ must also be unitary for our purposes.

Can you guess why these assumptions have to be made in order to implement this expression using quantum circuits?

Nevertheless, let's stick with these assumptions. Conveniently,

$$
\frac{\partial}{\partial\theta_i} U_i = icGU_i
$$

Therefore,

$$
\frac{\partial}{\partial\theta_i} f(\vec\theta) = \bra 0 U_1^\dagger \cdots \left( -icG^\dagger U^\dagger_i \right) \cdots U^\dagger_n A U(\vec\theta) \ket 0 \\
+ \bra 0 U^\dagger(\vec\theta) A U_n \cdots \left( icG U_i \right) \cdots U_1 \ket 0\\
= c\left[ \bra 0 U_1^\dagger \cdots \left( -iG^\dagger U^\dagger_i \right) \cdots U^\dagger_n A U(\vec\theta) \ket 0 \\
+ \bra 0 U^\dagger(\vec\theta) A U_n \cdots \left( iG U_i \right) \cdots U_1 \ket 0 \right]
$$

Each term in the expression above can be boiled down to the following:

$$
\frac{\partial}{\partial\theta_i} f(\vec\theta) = c\left[ \bra{\text{bra}_1} \text{ket}_1\rangle + \bra{\text{bra}_2} \text{ket}_2\rangle \right]
$$

where the freedom for how to define each bra and ket is left to you! Surely, if four PennyLane circuits are made that create each bra and ket, the derivative can be calculated with two inner products added together and multiplied by a coefficient $c$...

For the keen readers: do you actually need _four_ circuits 🧐?

Challenge code
--------------

In the code below, you are given a few functions:

*   `generator_info`: This function returns the generator $G$ and coefficient $c$ given an operator $U$.
*   `derivative`: This is where you will calculate the derivative of $f(\vec\theta)$ with respect to one parameter. Within this function, there are several circuits that you must complete:
    *   `circuit_bra1`: calculates $\bra{\text{bra}_1}$ (or $\ket{\text{bra}_1}$ depending on how you want to do things!)
    *   `circuit_ket1`: calculates $\ket{\text{ket}_1}$
    *   `circuit_bra2`: calculates $\bra{\text{bra}_2}$ (or $\ket{\text{bra}_2}$ depending on how you want to do things!)
    *   `circuit_ket1`: calculates $\ket{\text{ket}_2}$

### Inputs

As input to this problem, you are given:

*   `op_order` (`list(int)`): Each gate $U_i$ is one of the Pauli rotation gates: `qml.RX`, `qml.RY`, or `qml.RZ`. `op_order` defines the sequence of Pauli rotation gates that defines the circuit $U(\vec\theta)$.
*   `params` (`list(float)`): This is a list of parameters corresponding to $\vec\theta$.
*   `diff_idx`: This is an integer that relates to the parameter that will be differentiated with respect to.
*   `wires` (`list(int)`): This is a list of wires that each rotation gate will be applied to.
*   `measured_wire` (`int`): The wire that the observable $A$ will be applied to.

In our case, $A=Z$, the Pauli Z operator.

### Outputs

This code will output the derivative, a `float`, of the circuit with respect to the given parameter.

If your solution matches the correct one within the given tolerance specified in `check` (in this case it's a `1e-4` relative error tolerance), the output will be `"Correct!"`. Otherwise, you will receive a `"Wrong answer"` prompt.

Good luck!