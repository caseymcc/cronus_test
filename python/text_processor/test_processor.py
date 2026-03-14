"""Tests for the text processor module."""

import unittest
import sys
import os

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from text_processor.processor import word_count, truncate, title_case, reverse_words, extract_emails


class TestWordCount(unittest.TestCase):

    def test_simple(self):
        self.assertEqual(word_count("hello world"), {"hello": 1, "world": 1})

    def test_repeated_words(self):
        self.assertEqual(word_count("the cat and the dog"), {"the": 2, "cat": 1, "and": 1, "dog": 1})

    def test_punctuation_stripped(self):
        # "hello," should be counted as "hello"
        result = word_count("hello, hello world!")
        self.assertEqual(result["hello"], 2)
        self.assertNotIn("hello,", result)
        self.assertNotIn("world!", result)

    def test_mixed_case(self):
        result = word_count("Hello hello HELLO")
        self.assertEqual(result["hello"], 3)

    def test_empty(self):
        self.assertEqual(word_count(""), {})


class TestTruncate(unittest.TestCase):

    def test_short_text_unchanged(self):
        # Text shorter than max_length should NOT be truncated
        self.assertEqual(truncate("hi", 10), "hi")

    def test_exact_length_unchanged(self):
        self.assertEqual(truncate("hello", 5), "hello")

    def test_long_text_truncated(self):
        result = truncate("hello world", 5)
        self.assertEqual(result, "he...")  # 5 total: 2 chars + "..."

    def test_custom_suffix(self):
        result = truncate("hello world", 7, suffix="…")
        self.assertEqual(result, "hello …")

    def test_zero_length(self):
        self.assertEqual(truncate("hello", 0), "...")


class TestTitleCase(unittest.TestCase):

    def test_simple(self):
        self.assertEqual(title_case("hello world"), "Hello World")

    def test_articles_lowercase(self):
        # Articles/prepositions stay lowercase unless first word
        self.assertEqual(title_case("the lord of the rings"), "The Lord of the Rings")

    def test_first_word_always_capitalized(self):
        self.assertEqual(title_case("a tale of two cities"), "A Tale of Two Cities")

    def test_single_word(self):
        self.assertEqual(title_case("python"), "Python")


class TestReverseWords(unittest.TestCase):

    def test_basic(self):
        self.assertEqual(reverse_words("hello world"), "world hello")

    def test_single_word(self):
        self.assertEqual(reverse_words("hello"), "hello")

    def test_empty(self):
        self.assertEqual(reverse_words(""), "")

    def test_multiple_words(self):
        self.assertEqual(reverse_words("one two three"), "three two one")


class TestExtractEmails(unittest.TestCase):

    def test_basic(self):
        text = "Contact us at support@example.com for help"
        self.assertEqual(extract_emails(text), ["support@example.com"])

    def test_multiple(self):
        text = "Email alice@test.com or bob@test.com"
        result = extract_emails(text)
        self.assertIn("alice@test.com", result)
        self.assertIn("bob@test.com", result)

    def test_email_in_parentheses(self):
        text = "Contact (admin@example.com) for details"
        result = extract_emails(text)
        # Should extract clean email without parentheses
        self.assertIn("admin@example.com", result)
        self.assertNotIn("(admin@example.com)", result)

    def test_no_emails(self):
        self.assertEqual(extract_emails("no emails here"), [])


if __name__ == "__main__":
    unittest.main()
