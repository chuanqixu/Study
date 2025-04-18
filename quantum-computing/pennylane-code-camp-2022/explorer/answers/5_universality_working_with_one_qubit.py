import functools
import json
import math
import pandas as pd
import pennylane as qml
import pennylane.numpy as np
import scipy

np.random.seed(1967)


def get_matrix(params):
    """
    Args:
        - params (array): The four parameters of the model.
    Returns:
        - (matrix): The associated matrix to these parameters.
    """

    alpha, beta, gamma, phi = params

    # Put your code here #
    # Note: Another confusing thing of the question. e^{i\phi}* really means 
    # multiplied a global phase by np.exp(1j*phi) *, which is strange.
    # However, in the question, it actually ask you to use `qml.PhaseShift`.
    return qml.PhaseShift.compute_matrix(phi) * qml.RZ.compute_matrix(gamma) @ qml.RX.compute_matrix(beta) @ qml.RZ.compute_matrix(alpha)


def error(U, params):
    """
    This function determines the similarity between your generated matrix and the target unitary.

    Args:
        - U (matrix): Goal matrix that we want to approach.
        - params (array): The four parameters of the model.

    Returns:
        - (float): Error associated with the quality of the solution.
    """

    matrix = get_matrix(params)

    # Put your code here #
    matrix_diff = np.array(U, dtype="complex") - matrix
    # Notice: need to consider both the real and imaginary part
    return np.linalg.norm(np.real(matrix_diff)) + np.linalg.norm(np.imag(matrix_diff))



def train_parameters(U):

    epochs = 1000
    lr = 0.01

    grad = qml.grad(error, argnum=1)
    params = np.random.rand(4) * np.pi

    for epoch in range(epochs):
        params -= lr * grad(U, params)

    return params


# These functions are responsible for testing the solution.

def run(test_case_input: str) -> str:
    matrix = json.loads(test_case_input)
    params = [float(p) for p in train_parameters(matrix)]
    return json.dumps(params)

def check(solution_output: str, expected_output: str) -> None:
    matrix1 = get_matrix(json.loads(solution_output))
    matrix2 = json.loads(expected_output)
    assert not np.allclose(get_matrix(np.random.rand(4)), get_matrix(np.random.rand(4)))
    assert np.allclose(matrix1, matrix2, atol=0.2)


test_cases = [['[[ 0.70710678,  0.70710678], [ 0.70710678, -0.70710678]]', '[[ 0.70710678,  0.70710678], [ 0.70710678, -0.70710678]]'], ['[[ 1,  0], [ 0, -1]]', '[[ 1,  0], [ 0, -1]]']]

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
