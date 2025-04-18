import functools
import json
import math
import pandas as pd
import pennylane as qml
import pennylane.numpy as np
import scipy

def generator_info(operator):
    """Provides the generator of a given operator.

    Args:
        operator (qml.ops): A PennyLane operator

    Returns:
        (qml.ops): The generator of the operator.
        (float): The coefficient of the generator.
    """
    gen = qml.generator(operator, format="observable")
    return gen.ops[0], gen.coeffs[0]


def derivative(op_order, params, diff_idx, wires, measured_wire):
    """A function that calculates the derivative of a circuit w.r.t. one parameter.

    NOTE: you cannot use qml.grad in this function.

    Args:
        op_order (list(int)):
            This is a list of integers that defines the circuit in question.
            The entries of this list correspond to dictionary keys to op_dict.
            For example, [1,0,2] means that the circuit in question contains
            an RY gate, an RX gate, and an RZ gate in that order.

        params (np.array(float)):
            The parameters that define the gates in the circuit. In this case,
            they're all rotation angles.

        diff_idx (int):
            The index of the gate in the circuit that is to be differentiated
            with respect to. For instance, if diff_idx = 2, then the derivative
            of the third gate in the circuit will be calculated.

        wires (list(int)):
            A list of wires that each gate in the circuit will be applied to.

        measured_wire (int):
            The expectation value that needs to be calculated is with respect
            to the Pauli Z operator. measured_wire defines what wire we're
            measuring on.

    Returns:
        float: The derivative evaluated at the given parameters.
    """
    op_dict = {0: qml.RX, 1: qml.RY, 2: qml.RZ}
    dev = qml.device("default.qubit", wires=2)

    obs = qml.PauliZ(measured_wire)
    operator = op_dict[op_order[diff_idx]](params[diff_idx], wires[diff_idx])
    gen, coeff = generator_info(operator)

    @qml.qnode(dev)
    def circuit_bra1():

        # Put your code here #
        for op_idx, (op_label, op_param, wire) in enumerate(zip(op_order, params, wires)):
            if op_idx == diff_idx:
                qml.apply(gen)
            op_dict[op_label](op_param, wire)
        return qml.state()

    @qml.qnode(dev)
    def circuit_ket1():

        # Put your code here #
        for op_label, op_param, wire in zip(op_order, params, wires):
            op_dict[op_label](op_param, wire)
        # qml.apply(obs)
        return qml.state()

    @qml.qnode(dev)
    def circuit_bra2():

        # Put your code here #
        for op_label, op_param, wire in zip(op_order, params, wires):
            op_dict[op_label](op_param, wire)
        return qml.state()

    @qml.qnode(dev)
    def circuit_ket2():

        # Put your code here #
        for op_idx, (op_label, op_param, wire) in enumerate(zip(op_order, params, wires)):
            op_dict[op_label](op_param, wire)
            if op_idx == diff_idx:
                qml.apply(gen)
        # qml.apply(obs)
        return qml.state()

    bra1 = circuit_bra1()
    ket1 = circuit_ket1()
    bra2 = circuit_bra2()
    ket2 = circuit_ket2()

    # below code can only use 3 circuits
    # should not apply observable at the end of each ket circuit
    # first specify the observable A
    A = obs if measured_wire == 0 else qml.Identity(wires=0)
    for wire in range(1, dev.num_wires):
        A = A @ obs if wire == measured_wire else A @ qml.Identity(wire)
    a = np.matmul(np.matmul(bra1.conj(), A.matrix()), ket1)
    b = np.matmul(np.matmul(bra2.conj(), A.matrix()), ket2)
    
    # below code uses 4 circuits
    # should apply observable at the end of each ket circuit
    # a = np.matmul(bra1.conj(), ket1)
    # b = np.matmul(bra2.conj(), ket2)

    return np.real(1j * coeff * (-a + b)) # Put your code here #


# These functions are responsible for testing the solution.

def run(test_case_input: str) -> str:
    op_order, params, diff_idx, wires, measured_wire = json.loads(test_case_input)
    params = np.array(params, requires_grad=True)
    der = derivative(op_order, params, diff_idx, wires, measured_wire)
    return str(der)

def check(solution_output: str, expected_output: str) -> None:
    solution_output = json.loads(solution_output)
    expected_output = json.loads(expected_output)
    assert np.allclose(
        solution_output, expected_output, rtol=1e-4
    ), "Your derivative isn't quite right!"


test_cases = [['[[1,0,2,1,0,1], [1.23, 4.56, 7.89, 1.23, 4.56, 7.89], 0, [1, 0, 1, 1, 1, 0], 1]', '-0.2840528']]

for i, (input_, expected_output) in enumerate(test_cases):
    print(f"Running test case {i} with input '{input_}'...")

    try:
        output = run(input_)

    except Exception as exc:
        print(f"Runtime Error. {exc}")

    else:
        if message := check(output, expected_output):
            print(f"Wrong Answer. Have: '{output}'. Want: '{expected_output}'.")

        else:
            print("Correct!")