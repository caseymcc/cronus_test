/**
 * Tests for StringUtils.
 *
 * Compile: g++ -std=c++17 -o test_string_utils test_string_utils.cpp string_utils.cpp && ./test_string_utils
 */

#include "string_utils.h"
#include <cassert>
#include <iostream>

int testsRun = 0;
int testsPassed = 0;

#define TEST(name) \
    void name(); \
    struct name##_reg { name##_reg() { std::cout << "  " << #name << "... "; testsRun++; try { name(); testsPassed++; std::cout << "PASS\n"; } catch(const std::exception &e) { std::cout << "FAIL: " << e.what() << "\n"; } catch(...) { std::cout << "FAIL (unknown exception)\n"; } } } name##_instance; \
    void name()

void check(bool condition, const char *msg)
{
    if(!condition)
    {
        throw std::runtime_error(msg);
    }
}

TEST(test_trim_spaces)
{
    check(StringUtils::trim("  hello  ") == "hello", "Should trim spaces");
    check(StringUtils::trim("hello") == "hello", "No-op for clean string");
    check(StringUtils::trim("") == "", "Empty string");
}

TEST(test_trim_tabs_and_newlines)
{
    check(StringUtils::trim("\thello\t") == "hello", "Should trim tabs");
    check(StringUtils::trim("\nhello\n") == "hello", "Should trim newlines");
    check(StringUtils::trim(" \t\nhello\n\t ") == "hello", "Should trim mixed whitespace");
}

TEST(test_to_lower)
{
    check(StringUtils::toLower("Hello World") == "hello world", "Basic toLower");
    check(StringUtils::toLower("ALLCAPS") == "allcaps", "All caps");
    check(StringUtils::toLower("already") == "already", "Already lower");
}

TEST(test_to_upper)
{
    check(StringUtils::toUpper("Hello World") == "HELLO WORLD", "Basic toUpper");
    check(StringUtils::toUpper("lower") == "LOWER", "All lower");
}

TEST(test_split_basic)
{
    auto parts = StringUtils::split("a,b,c", ',');
    check(parts.size() == 3, "Should split into 3 parts");
    check(parts[0] == "a" && parts[1] == "b" && parts[2] == "c", "Parts should be a, b, c");
}

TEST(test_split_trailing_delimiter)
{
    auto parts = StringUtils::split("a,b,", ',');
    // "a,b," should produce ["a", "b", ""] — the trailing empty string matters
    check(parts.size() == 3, "Trailing delimiter should produce empty final element");
    check(parts[2] == "", "Last element should be empty string");
}

TEST(test_split_no_delimiter)
{
    auto parts = StringUtils::split("hello", ',');
    check(parts.size() == 1, "No delimiter should give 1 part");
    check(parts[0] == "hello", "Single part should be whole string");
}

TEST(test_join_basic)
{
    std::vector<std::string> parts = {"a", "b", "c"};
    check(StringUtils::join(parts, ", ") == "a, b, c", "Basic join");
}

TEST(test_join_no_trailing_delimiter)
{
    std::vector<std::string> parts = {"hello", "world"};
    std::string result = StringUtils::join(parts, " ");
    check(result == "hello world", "Join should not add trailing delimiter");
    check(result.back() != ' ', "Last char should not be delimiter");
}

TEST(test_join_empty)
{
    std::vector<std::string> parts;
    check(StringUtils::join(parts, ",") == "", "Empty join");
}

TEST(test_join_single)
{
    std::vector<std::string> parts = {"only"};
    check(StringUtils::join(parts, ",") == "only", "Single element join");
}

TEST(test_starts_with)
{
    check(StringUtils::startsWith("hello world", "hello"), "Basic startsWith");
    check(!StringUtils::startsWith("hello", "world"), "Not starts with");
    check(StringUtils::startsWith("hello", ""), "Empty prefix");
    check(!StringUtils::startsWith("hi", "hello"), "Prefix longer than string");
}

TEST(test_ends_with)
{
    check(StringUtils::endsWith("hello world", "world"), "Basic endsWith");
    check(!StringUtils::endsWith("hello", "world"), "Not ends with");
    check(StringUtils::endsWith("hello", ""), "Empty suffix");
}

TEST(test_replace)
{
    check(StringUtils::replace("hello world", "world", "there") == "hello there", "Basic replace");
    check(StringUtils::replace("aaa", "a", "bb") == "bbbbbb", "Multiple replacements");
    check(StringUtils::replace("hello", "xyz", "abc") == "hello", "No match");
}

int main()
{
    std::cout << "\nString Utils Tests:\n";
    std::cout << "\n" << testsPassed << "/" << testsRun << " tests passed.\n";

    return (testsPassed == testsRun) ? 0 : 1;
}
