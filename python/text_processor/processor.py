"""
Text processor with string manipulation utilities.

BUG: word_count doesn't handle punctuation properly.
BUG: truncate doesn't handle edge case where text is shorter than max_length.
BUG: title_case incorrectly capitalizes articles/prepositions.
"""


def word_count(text):
    """Count words in text, ignoring punctuation.

    BUG: Doesn't strip punctuation, so "hello," and "hello" are different words.
    """
    words = text.split()
    counts = {}
    for word in words:
        # BUG: should strip punctuation and lowercase before counting
        word = word.lower()
        counts[word] = counts.get(word, 0) + 1
    return counts


def truncate(text, max_length, suffix="..."):
    """Truncate text to max_length, adding suffix if truncated.

    BUG: Doesn't handle case where max_length is greater than text length.
    BUG: Doesn't account for suffix length in the total.
    """
    if max_length <= 0:
        return suffix
    # BUG: always adds suffix even when text fits
    return text[:max_length] + suffix


def title_case(text):
    """Convert text to title case.

    Simple articles and prepositions should stay lowercase unless
    they are the first word.

    BUG: Currently just calls .title() which capitalizes everything.
    """
    return text.title()


def reverse_words(text):
    """Reverse the order of words in text, preserving word content."""
    words = text.split()
    return " ".join(reversed(words))


def extract_emails(text):
    """Extract email addresses from text.

    BUG: The simple split-based approach misses emails with punctuation around them.
    """
    import re
    # BUG: Overly simple pattern misses valid emails and matches invalid ones
    pattern = r'\S+@\S+'
    matches = re.findall(pattern, text)
    return matches
