import functools
import json
import math
import pandas as pd
import pennylane as qml
import pennylane.numpy as np
import scipy

def fourier_decomp(layers_params):
    """
    Returns the frequencies and coefficient of our quantum model, specified by layers_params

    Args:
    layers_params: list(list(list(float))). Specifies the number of basic entangling layers and their
    parameters as explained in the statement of the problem.

    Returns: list([float,float,float]). A list three-element list. The first element of each list is the frequency. The second
    element is the real part of the coefficient associated with that frequency in the Fourier decomposition. The third element
    is the imaginary part of such coefficient.
    """

    dev = qml.device("default.qubit", wires=4)

    @qml.qnode(dev)
    def circuit(layers_params, *any_additional_arguments_you_need):
        """
        This function is the quantum circuit made of alternating entangling layers and rotations representing our quantum model
        """
        # Put your code here #
        # Return a single expectation value!

    # Use the Fourier module to obtain the coefficients and frequencies. Then return the required list of lists.


# These functions are responsible for testing the solution.

def run(test_case_input: str) -> str:

    ins = json.loads(test_case_input)
    output = fourier_decomp(ins)

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
    ), "Your calculated Fourier spectrum isn't quite right."


test_cases = [['[[[2,2,2,2],[1,2,1,1]],[[3,4,5,6]]]', '[[-4.0, -2.4671622769447922e-17, -1.2335811384723961e-17], [-3.0, -0.03395647263976357, 0.010208410500915437], [-2.0, 2.8360500437920326e-17, 1.850371707708594e-17], [-1.0, 0.11762992558035439, -0.13619443127813127], [0.0, 8.018277400070575e-17, 0.0], [1.0, 0.11762992558035439, 0.13619443127813124], [2.0, 3.700743415417188e-17, -1.850371707708594e-17], [3.0, -0.03395647263976357, -0.010208410500915437],[4.0, -3.688877668472405e-18, 1.850371707708594e-17]]'], ['[[[2,2,2,2]],[[3,4,5,6]]]', '[[-4.0, 1.2335811384723961e-17, 3.700743415417188e-17],  [-3.0, 0.022482345076620468, -0.07855141721016852], [-2.0, -1.2335811384723961e-17, -6.536793459209221e-17], [-1.0, -0.13243693333822854, 0.17097830099559677], [0.0, -2.4671622769447922e-17, 0.0], [1.0, -0.13243693333822854, -0.17097830099559677], [2.0, -2.4671622769447922e-17, 7.401486830834377e-17], [3.0, 0.022482345076620468, 0.07855141721016852], [4.0, -1.2335811384723961e-17, -3.331855648569948e-17]]']]

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