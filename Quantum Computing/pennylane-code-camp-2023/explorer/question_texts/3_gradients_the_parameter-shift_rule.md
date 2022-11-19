# Gradients: The parameter-shift rule


There are many ways of differentiating quantum circuits. One such way that is also hardware-compatible (i.e., it can be computed using a real quantum computer) is the parameter-shift rule. Here, we'll explore using the parameter-shift rule in the simplest case: for differentiating the Pauli rotation gates.

Consider a circuit given by a unitary $U(\vec \theta)$, where $\vec \theta = (\theta_1, ..., \theta_n)$ are real differentiable parameters, whose output is an expectation value with respect to an observable $A$:

$$
f(\vec \theta) = \bra 0 U^\dagger(\vec \theta) A U(\vec \theta) \ket 0
$$

If the parameterized gates in $U(\vec \theta)$ were only composed of Pauli rotation gates ($RX$, $RY$, and $RZ$), then differentiating $f(\vec \theta)$ with respect to one of the parameters can be shown to simplify to

$$
\frac{\partial}{\partial\theta_i}f(\vec \theta) = \frac{f(\theta_1, ..., \theta_i+s, ..., \theta_n)-f(\theta_1, ..., \theta_i-s, ..., \theta_n)}{2\sin(s)}
$$

where $s$ is a finite-valued _shift_. Although this looks eerily similar to differentiation via finite-differences, $s$ need not be infinitesimally small! In this problem, you will implement the parameter-shift rule for the Pauli rotation gates yourself.

Challenge code
--------------

In the provided code, you are given a couple of functions:

*   `circuit`: This describes the function $f(\vec \theta)$.
*   `my_parameter_shift_grad`: This is where you will implement calculating $\frac{\partial}{\partial\theta_i}f(\vec \theta)$ for every $\theta_i\in\vec \theta$. It will return the gradient, which is a vector containing every partial derivative. **You must complete this function**.

### Input

As input to this problem, you are given:

*   `params` (`list(float)`): a list corresponding to $\vec \theta$.
*   `shift` (`float`): the shift amount, corresponding to $s$.

### Output

This code will output the `gradient` (`list(float)`) of the `circuit`.

If your solution matches the correct one within the given tolerance specified in `check` (in this case it's a `1e-4` relative error tolerance), the output will be `"Correct!"`. Otherwise, you will receive a `"Wrong answer"` prompt.

Good luck!