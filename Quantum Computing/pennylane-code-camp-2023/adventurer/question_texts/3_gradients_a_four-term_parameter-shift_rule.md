# Gradients: A four-term parameter-shift rule

In the last problem, you learned about the two-term parameter-shift rule for the Pauli rotation gates. The parameter-shift rule isn't limited to the Pauli rotation gates, and in this problem you'll adventure into more complicated parameter shift rules!

Arguably the next simplest parameter-shift rule compared to the Pauli rotation gates is that of the _controlled_ Pauli rotation gates: $CRX$, $CRY$, and $CRZ$. Let's highlight the key differences between the parameter-shift rule for the controlled Pauli rotations and the regular Pauli rotations.

*   Four terms: the parameter-shift rule for the controlled Pauli rotations contains _four_ terms, not two!
*   Shifts: for the parameter-shift rule for the Pauli rotations, we could use any real-valued shift for it to work. Here, it's not so simple! The coefficients of the shift rule and the shifts themselves depend on each other!

The parameter-shift rule looks something like this:

$$
\frac{\partial}{\partial \theta_i} f(\vec \theta) = c_1 \left( f(\theta_i+s_1) - f(\theta_i-s_1) \right) - c_2 \left( f(\theta_i+s_2) - f(\theta_i-s_2) \right)
$$

where the shifts $s_i$ and coefficients $c_i$ depend on each other. You'll have to do some digging to figure out how to calculate them... but there isn't a unique solution!

Challenge code
--------------

In the code below, you are given a few functions:

*   `circuit`: This describes the function $f(\vec \theta)$.
*   `shifts_and_coeffs`: This function calculates $s_1$, $s_2$, $c_1$, and $c_2$. No fancy calculations are needed here; you can simply state, for example, $s_1=1.23$ and $s_2=4.56$ and return those values. **You must complete this function**.
*   `my_parameter_shift_grad`: This is where you will implement calculating $\frac{\partial}{\partial \theta_i} f(\vec \theta)$ for every $\theta_i\in \vec\theta$. It will return the gradient, which is a vector containing every partial derivative. **You must complete this function**.

### Input

As input to this problem, you are given `params` (`list(float)`), which correspond to the differentiable circuit parameters $\vec\theta$.

### Output

This code will output the `gradient` (`list(float)`) of the `circuit`.

If your solution matches the correct one within the given tolerance specified in `check` (in this case it's a `1e-4` relative error tolerance), the output will be `"Correct!"`. Otherwise, you will receive a `"Wrong answer"` prompt.

Good luck!