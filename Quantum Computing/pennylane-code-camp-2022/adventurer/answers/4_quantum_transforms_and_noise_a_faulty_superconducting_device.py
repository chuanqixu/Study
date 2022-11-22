import functools
import json
import math
import pandas as pd
import pennylane as qml
import pennylane.numpy as np
import scipy

sample = np.array([[0.88645553, 5.84095018],
        [1.76306821, 1.28529014],
        [2.09187193, 6.06022346],
        [2.7455983 , 4.10650061],
        [1.156449  , 0.05270523],
        [2.41036002, 4.1243531 ],
        [1.85975128, 6.06105961],
        [1.21447324, 4.50789405],
        [2.98670697, 2.76236853],
        [0.85240889, 4.25292296],
        [2.51060472, 0.16869898],
        [1.62333082, 0.1550452 ],
        [2.48189743, 3.74707815],
        [1.57124183, 4.57583136],
        [0.97387373, 5.09057019],
        [1.48945306, 1.62557981],
        [1.63373765, 4.45798143],
        [2.59893856, 1.05395551],
        [1.55004825, 3.8295616 ],
        [2.53605197, 2.37092827]])


def average_fidelity(gate_list, wire_list, noise_param, over_rot):

    """This function returns the average fidelity of a noisy superconducting circuit
    with respect to the ideal version of such circuit, which contains CNOT gates.

    Args:
        - gate_list (list(str)): Gates, assumed to be non-parametric, contained in the ideal circuit. 
        - wire_list (list(int)): Wires on which each of the gates in gate_list act
        - noise_param (float): The noise parameter characterizing the depolarizing gate after the sqrt(iSWAP) gates
        - over_rot (float): Extra rotation angle on each rotation gate.
    Returns: 
        - (float): Average fidelity of the superconducting circuit with respect to the ideal circuit.
    """

    
    def circuit():
        """This function creates a circuit from gate_list and wire_list (You shouldn't modify this)"""
        for i in range(len(gate_list)):
            getattr(qml,gate_list[i])(wires=wire_list[i])
        
    ref_device = qml.device('default.mixed', wires = 2)

    @qml.qnode(ref_device)
    def reference_circuit(phi, theta):
        """Ideal circuit, with prior state preparation (You shouldn't modify this)"""
        qml.Rot(phi, theta, 0, wires = 0)
        circuit()
        return qml.state()
    
    dev = qml.device('default.mixed', wires = 2)
    
    def custom_cnot(param,wires):
        # Put your code here #
        # Return your custom CNOT gate as implemented by a superconducting qubit as an array, including noise
        # See documentation for more details!

        # it is confusing that `default.mixed` does not support ISWAP, so ISWAP needs
        # to be represented as CNOT, which will in turn replaced by ISWAP again and 
        # cause an infinite recursive loop.
        # However, the expected values in the tests are the values that directly use 
        # qml.ISWAP

        return [
            qml.RZ(-np.pi/2+param[1], wires=wires[0]),
            qml.RX(np.pi/2+param[1], wires=wires[1]),
            qml.RZ(np.pi/2+param[1], wires=wires[1]),
            # qml.QubitUnitary(qml.ISWAP.compute_matrix(), wires=[wires[0], wires[1]]),
            qml.ISWAP(wires=[wires[0], wires[1]]),
            qml.RX(np.pi/2+param[1], wires=wires[0]),
            # qml.QubitUnitary(qml.ISWAP.compute_matrix(), wires=[wires[0], wires[1]]),
            qml.ISWAP(wires=[wires[0], wires[1]]),
            qml.RZ(np.pi/2+param[1], wires=wires[1]),
            qml.DepolarizingChannel(param[0], wires=wires[0]),
            qml.DepolarizingChannel(param[0], wires=wires[1])
        ]
    
    # custom_decomps = # Write here the custom decomposition of the CNOT gate for your device.
    custom_cnot_bind = functools.partial(custom_cnot, [noise_param, over_rot])
    custom_decomps = {qml.CNOT: custom_cnot_bind}
    expand_fn = qml.transforms.create_decomp_expand_fn(custom_decomps, dev)
    dev.custom_expand(expand_fn) 
    
    @qml.qnode(dev, expansion_strategy = "device")
    def superconducting_circuit(phi,theta):
        qml.Rot(phi, theta, 0, wires = 0)
        circuit()
        return qml.state()
    
    # print(qml.draw(reference_circuit)(sample[0][1], sample[0][0]))
    # print(qml.draw(superconducting_circuit)(sample[0][1], sample[0][0]))

    def fidelity(phi,theta):
        """Return the fidelity of the final state of the circuit, with respect to the ideal circuit, given
        an initial state parametrized by phi and theta"""
        # Put your code here #
        return qml.math.fidelity(superconducting_circuit(phi,theta), reference_circuit(phi, theta))

    # Write any other helper functions here, if you need any.

    # Return the average fidelity by running over the sample states given to you in the list "sample"
    return np.mean([fidelity(angles[1], angles[0]) for angles in sample])


# These functions are responsible for testing the solution.

def run(test_case_input: str) -> str:
		
    ins = json.loads(test_case_input)
    output = average_fidelity(*ins)

    return str(output) 

def check(solution_output: str, expected_output: str) -> None:
    """
    Compare solution with expected.

    Args:
            solution_output: The output from an evaluated solution. Will be
            the same type as returned.
            expected_output: The correct result for the test case.

    Raises: 
            ``AssertionError`` if the solution output is incorrect in any way.
    """

    solution_output = json.loads(solution_output)
    expected_output = json.loads(expected_output)
    assert np.allclose(
        solution_output, expected_output, rtol=1e-2
    ), "Your calculated average fidelity isn't quite right."


test_cases = [['[["Hadamard","CNOT","PauliZ"],[0,[0,1],0],0.0001,0.01]', '0.363157'], ['[["CNOT","PauliZ","PauliZ"],[[1,0],1,0],0.0001,0.01]', '0.445426']]

for i, (input_, expected_output) in enumerate(test_cases):
    print(f"Running test case {i} with input '{input_}'...")
    # output = run(input_)
    try:
        output = run(input_)

    except Exception as exc:
        print(f"Runtime Error. {exc}")

    else:
        if message := check(output, expected_output):
            print(f"Wrong Answer. Have: '{output}'. Want: '{expected_output}'.")

        else:
            print("Correct!")