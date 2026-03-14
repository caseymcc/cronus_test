"""
Calculator module — delegates to operations.py for actual math.

BUG: Division result is cast to int, losing precision.
BUG: Chained operations don't reset state properly.
"""


class Calculator:
    def __init__(self):
        self.result = 0
        self.history = []

    def add(self, a, b):
        from .operations import add
        self.result = add(a, b)
        self.history.append(("add", a, b, self.result))
        return self.result

    def subtract(self, a, b):
        from .operations import subtract
        self.result = subtract(a, b)
        self.history.append(("subtract", a, b, self.result))
        return self.result

    def multiply(self, a, b):
        from .operations import multiply
        self.result = multiply(a, b)
        self.history.append(("multiply", a, b, self.result))
        return self.result

    def divide(self, a, b):
        from .operations import divide
        self.result = divide(a, b)
        self.history.append(("divide", a, b, self.result))
        return self.result

    def chain(self, operations):
        """Apply a list of (operation, value) tuples to self.result.

        Example: calc.chain([("add", 5), ("multiply", 3)])
        starts from self.result and applies each operation.
        """
        for op, value in operations:
            if op == "add":
                self.result = self.result + value
            elif op == "subtract":
                self.result = self.result - value
            elif op == "multiply":
                self.result = self.result * value
            elif op == "divide":
                self.result = self.result / value
            else:
                raise ValueError(f"Unknown operation: {op}")
            self.history.append((op, self.result, value, self.result))
        return self.result

    def clear(self):
        """Reset calculator state."""
        self.result = 0
        # BUG: history is not cleared
        return self.result

    def get_history(self):
        return list(self.history)
