import functools
import json
import math
import pandas as pd
import pennylane as qml
import pennylane.numpy as np
import scipy

# Welcome to PennyLane Code Camp!


def add_numbers(x, y):
    """This function adds two numbers together.

    Args:
        x (float): A number.
        y (float): A number.

    Returns:
        (float): The result of adding x and y.
    """
    return x+y


# These functions are responsible for testing the solution.

def run(test_case_input: str) -> str:
    x, y = json.loads(test_case_input)
    result = add_numbers(x, y)
    return str(result)

def check(solution_output: str, expected_output: str) -> None:
    solution_output = json.loads(solution_output)
    expected_output = json.loads(expected_output)
    assert np.allclose(
        solution_output, expected_output, rtol=1e-4
    ), "Your addition function isn't quite right!"


test_cases = [['[0.2, -0.2]', '0.0']]

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
