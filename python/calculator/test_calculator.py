"""Tests for the calculator module.

These tests define the CORRECT behavior. A proper fix to the bugs
in calculator.py and operations.py will make all tests pass.
"""

import unittest
import sys
import os

# Add parent directory to path for imports
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from calculator.calculator import Calculator
from calculator.operations import add, subtract, multiply, divide


class TestOperations(unittest.TestCase):
    """Test the low-level operations module."""

    def test_add(self):
        self.assertEqual(add(2, 3), 5)
        self.assertEqual(add(-1, 1), 0)
        self.assertEqual(add(0.1, 0.2), 0.1 + 0.2)

    def test_subtract(self):
        self.assertEqual(subtract(5, 3), 2)
        self.assertEqual(subtract(0, 5), -5)

    def test_multiply(self):
        self.assertEqual(multiply(3, 4), 12)
        self.assertEqual(multiply(-2, 3), -6)
        self.assertEqual(multiply(0, 100), 0)

    def test_divide(self):
        self.assertEqual(divide(10, 3), 10 / 3)  # Should be ~3.333, not 3
        self.assertEqual(divide(7, 2), 3.5)       # Should be 3.5, not 3
        self.assertEqual(divide(1, 4), 0.25)       # Should be 0.25, not 0

    def test_divide_by_zero(self):
        with self.assertRaises(ValueError):
            divide(5, 0)


class TestCalculator(unittest.TestCase):
    """Test the Calculator class."""

    def setUp(self):
        self.calc = Calculator()

    def test_basic_operations(self):
        self.assertEqual(self.calc.add(2, 3), 5)
        self.assertEqual(self.calc.subtract(10, 4), 6)
        self.assertEqual(self.calc.multiply(3, 7), 21)

    def test_divide_precision(self):
        result = self.calc.divide(10, 3)
        self.assertAlmostEqual(result, 3.333333, places=5)

    def test_chain_operations(self):
        self.calc.result = 10
        result = self.calc.chain([("add", 5), ("multiply", 2)])
        self.assertEqual(result, 30)  # (10 + 5) * 2 = 30

    def test_chain_with_divide(self):
        self.calc.result = 100
        result = self.calc.chain([("divide", 3)])
        self.assertAlmostEqual(result, 33.333333, places=5)

    def test_clear_resets_everything(self):
        self.calc.add(5, 3)
        self.calc.multiply(2, 4)
        self.calc.clear()
        self.assertEqual(self.calc.result, 0)
        self.assertEqual(self.calc.get_history(), [])  # BUG: history not cleared

    def test_history_tracking(self):
        self.calc.add(1, 2)
        self.calc.multiply(3, 4)
        history = self.calc.get_history()
        self.assertEqual(len(history), 2)
        self.assertEqual(history[0][0], "add")
        self.assertEqual(history[1][0], "multiply")


if __name__ == "__main__":
    unittest.main()
