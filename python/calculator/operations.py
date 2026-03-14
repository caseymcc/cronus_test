"""
Basic arithmetic operations.

BUG: divide() uses integer division instead of float division.
"""


def add(a, b):
    return a + b


def subtract(a, b):
    return a - b


def multiply(a, b):
    return a * b


def divide(a, b):
    if b == 0:
        raise ValueError("Cannot divide by zero")
    # BUG: integer division truncates result
    return a // b
