# Quantum transforms and noise: A faulty superconducting device

If you have ever tried to run a job on a real quantum device, you may have noticed that your results are not as accurate as you would've hoped for. Minimizing these errors is, in fact, one of the main goals of quantum engineering. Why do these errors happen? The reason is that **gates are noisy**. When a qubit goes through a gate, it gets entangled with the quantum state of the environment (i.e. our lab). This is an unavoidable experimental error, since we can't isolate a quantum system completely if we want to manipulate it. Therefore, real-life gates will not transform the qubits exactly as they do on paper.

It turns out that some devices may be less error-prone than others when executing a particular task. **Gate implementation is device dependent**, which means that, often times, devices need to implement a series of gates that is equivalent to the gate we need. The implementation of extra gates results in additional errors due to noise.

For example, suppose that we are given a superconducting device that cannot implement the _CNOT_ gate directly. Instead, the device can directly apply the _iSWAP_ gate by connecting two superconducting circuits through a capacitor. Not a problem! We can write the _CNOT_ gate in terms _iSWAP_ as follows:

![](../figs/dqt_circuit3.png)

The iSWAP gate in the above circuit introduces some noise, which we will model by adding one depolarizing channel with the at the end of each wire in the above circuit. Both depolarizing channels depend on the same noise parameter $\beta$. Additionally, we will assume that, due to a fabrication error, all of the rotation gates in the circuit above are overrotated by an angle $\theta$ (any rotation parameter $\alpha$ in the ideal circuit becomes $\alpha+\theta$ in the superconducting circuit).

Your task is to find the _average fidelity_ between an input circuit and the same circuit implemented in this (faulty) superconducting device, as a function of the parameters $\beta$ and $\theta$.

Challenge code
--------------

You must complete the `average_fidelity` function to find the average [fidelity](https://docs.pennylane.ai/en/stable/code/api/pennylane.math.fidelity.html?highlight=fidelity) between the superconducting circuit and the ideal circuit.

The most elegant way to solve this coding challenge is to use `qml.transforms.create_decomp_expand_fn` to modify the given device `dev`, so that it automatically implements the CNOT gate as if it were the superconducting device described above.

You will need to take the average of the fidelity of many states to calculate your output. To help you with this, a set `sample` of 20 uniformly distributed points on the Bloch sphere, parametrized by their azimuthal and polar angles $\theta$ and $\phi$, in the order $[\theta, \phi]$. That is the state is given by:

$$
\ket\psi = \cos(\frac{\theta}{2})\ket 0 + e^{i\phi} \sin(\frac{\theta}{2}) \ket 1
$$

This set has been generated using the Haar measure (don't worry too much about this, it's just a way to ensure that states are sampled without bias!). We then use a `Rot` gate to prepare the initial state. **Use this sample to calculate the average fidelity**; if you do not, your results may not match the expected output.

### Input

As input to this problem, you will be given:

*   `gate_list` array(str): A list of gates contained in the input circuit
*   `wire_list` array(arr(int)): List of lists of wires on which the gates in `gate_list` act, in order.
*   `noise_param` (float): The parameter $\beta$ of the depolarizing gate associated with the iSWAP gates.
*   `over_rot` (float): The systematic rotation error $\theta$ in the faulty implementation of the CNOT gate.

### Output

This code will output the average fidelity, a `float`, of the superconducting circuit with respect to the ideal circuit.

If your solution matches the correct one within the given tolerance specified in `check` (in this case it's an absolute tolerance of `0.01`), the output will be `"Correct!"`. Otherwise, you will receive a `"Wrong answer"` prompt.

Good luck!