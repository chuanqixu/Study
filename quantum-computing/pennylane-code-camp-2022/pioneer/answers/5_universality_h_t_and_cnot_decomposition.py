import functools
import json
import math
import pandas as pd
import pennylane as qml
import pennylane.numpy as np
import scipy

def U():
    """
    This quantum function will simply contain H, T and CNOT gates.
    It will not return any value.
    """

    # Put your solution here #

    # (1) SWAP = CNOT(0, 1)--CNOT(1, 0)--CNOT(0, 1)
    # 
    # (2) H--·--H   --⊕--           H--·--     --⊕--H
    #        |    =   |      ----->     |    =    |   
    #     H--⊕-H   --·--            H--⊕--    --·---H
    #
    # (3) ---·---    --·--
    #        |    =    |   
    #     H--⊕--H   --Z---
    #
    # Steps:
    # 1. First, because the first operation is the same as SWAP decomposition (CNOT(0, 1)),
    #    we can get rid of it, and compute the remaining part to be `CNOT(1, 0)--CNOT(0, 1)`.
    # 2. Suppose the rightmost operation in U is CNOT(1, 0), i.e., U = U'--CNOT(0, 1)
    #    This assumption is to make the rightmost two H gates inside, and put CNOT(0, 1) 
    #    at the end. Therefore, we can further get rid of CNOT(0, 1) again in the 
    #    decomposition.
    #    Also, CNOT(1, 0) in the first U cancels out CNOT(1, 0) in the middle of the original
    #    circuit. Thus, we transform the problem into:
    #    --H----------------H--     --⊕--
    #         | U'|   |U'|       =    |     
    #    --H----------------H--     --·-- 
    # 3. From equivalence (2) above, we know that U'^2= CNOT(0, 1), so U' = sqrt(CNOT(0, 1))
    # 4. sqrt(CNOT(0, 1)) can be computed with ABC decomposition:
    #    https://qiskit.org/textbook/ch-gates/more-circuit-identities.html#3.-Controlled-Rotations-
    # 5. I directly use the answer here:
    #    https://quantumcomputing.stackexchange.com/questions/16812/single-qubit-gates-or-cnot-sequence-to-transform-swap-to-sqrt-of-swap

    qml.T(wires=0)
    qml.Hadamard(wires=1)
    qml.T(wires=1)

    qml.CNOT(wires=[0, 1])
    
    for _ in range(7):
        qml.T(wires=1)
    
    qml.CNOT(wires=[0, 1])
    qml.Hadamard(wires=1)

    qml.CNOT(wires=[1, 0])



dev = qml.device("default.qubit", wires=2)

@qml.qnode(dev)
def circuit():
    """
    Main circuit given in the statement, here the operators you have defined in U will be embedded.
    """

    qml.CNOT(wires=[0, 1])

    qml.Hadamard(wires=0)
    qml.Hadamard(wires=1)

    U()

    qml.CNOT(wires=[1, 0])

    U()

    qml.Hadamard(wires=0)
    qml.Hadamard(wires=1)

    return qml.state()


# These functions are responsible for testing the solution.

def run(input: str) -> str:
    matrix = qml.matrix(circuit)().real

    with qml.tape.QuantumTape() as tape:
        U()

    names = [op.name for op in tape.operations]
    return json.dumps({"matrix": matrix.tolist(), "gates": names})

def check(user_output: str, expected_output: str) -> str:

    parsed_output = json.loads(user_output)
    matrix_user = np.array(parsed_output["matrix"])
    gates = parsed_output["gates"]

    assert np.allclose(matrix_user, qml.matrix(qml.SWAP)(wires=[0, 1]))
    assert (
        len(set(gates)) == 3
        and "Hadamard" in gates
        and "CNOT" in gates
        and "T" in gates
    )


test_cases = [['No input', 'No output']]

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