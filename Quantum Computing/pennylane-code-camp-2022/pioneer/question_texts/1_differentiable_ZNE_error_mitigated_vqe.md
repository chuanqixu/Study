# Differentiable ZNE: Error-mitigated VQE

At this point, you've been exposed to modelling noise in a quantum circuit and how to amplify a circuit's noise via global circuit folding. In this challenge, you will pioneer a way to perform an error-mitigated version of a quintessential quantum algorithm on noisy hardware!

The variational quantum eigensolver (VQE) algorithm is an elegant optimization algorithm that estimates the ground state and ground state energy of a Hamiltonian. The Hamiltonian could describe anything, so long as there's an efficient map to mapping the degrees of freedom to binary variables. Here, we'll use the qubit representation of the molecular hydrogen Hamiltonian. Go check out [our popular demo](https://pennylane.ai/qml/demos/tutorial_quantum_chemistry.html) on performing VQE with this Hamiltonian!

Here, you will perform:

*   an "ideal" VQE routine (no noise present at all)
*   three error-ridden VQE routines
*   an extrapolation to zero-noise given the three error-ridden VQE routines

![](../figs/noisy_vqe.png)

The three error-ridden VQE routines must be constructed from a noisy device, where a depolarizing channel is present on _all_ gates. There, you will employ PennyLane's builtin global circuit folding protocol that can be implemented via transforming a noisy QNode: [`qml.transforms.fold_global`](https://docs.pennylane.ai/en/stable/code/api/pennylane.transforms.fold_global.html?highlight=qml.transforms.fold_global).

Challenge code
--------------

The provided code below contains a few functions:

*   `hydrogen_hamiltonian`: this function creates the Hamiltonian describing the H molecule given a hydrogen-to-hydrogen distance `d`.
*   `ansatz_template`: this is a quantum function that defines the ansatz for all VQE routines that you will perform.
*   `VQE`: this function performs a VQE routine given a QNode that you created with `qnode_ansatzes`.
*   `qnode_ansatzes`: employs `ansatz_template` to create QNodes given different devices: an ideal device (no noise) and "mitigated" devices that are global-circuit-folding transformed devices. **You must complete this function**.
*   `extrapolation`: this is where the magic happens; all VQE routines run, and then the zero-noise extrapolation takes place. **You must complete this function**.

The three error-ridden VQE routines that you perform are tied to three different _scale factors_. The scale factors define the global circuit folding procedure, and more information can be found in the documentation for [`qml.transforms.fold_global`](https://docs.pennylane.ai/en/stable/code/api/pennylane.transforms.fold_global.html?highlight=qml.transforms.fold_global). When you perform the extrapolation, you will be fitting a second-degree polynomial function (a quadratic function) to VQE energies versus scale factors.

### Input

As input to this problem, you will be given `d`, the hydrogen-to-hydrogen distance that defines its molecular Hamiltonian.

### Output

This code will output two energies corresponding to the noise-free VQE routine and the extrapolated energy from the ZNE routine: `[ideal_energy, zne_energy]` (`list(float)`).

If your solution matches the correct one within the given tolerance specified in `check` (in this case it's a `0.01` relative error tolerance), the output will be `"Correct!"`. Otherwise, you will receive a `"Wrong answer"` prompt.

Good luck!