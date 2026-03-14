/**
 * Tests for LinkedList.
 *
 * Compile: g++ -std=c++17 -o test_linked_list test_linked_list.cpp linked_list.cpp && ./test_linked_list
 */

#include "linked_list.h"
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

TEST(test_push_back)
{
    LinkedList list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    check(list.size() == 3, "Expected size 3");
    check(list.at(0) == 1, "Expected first element 1");
    check(list.at(2) == 3, "Expected last element 3");
}

TEST(test_push_front)
{
    LinkedList list;
    list.pushFront(1);
    list.pushFront(2);
    list.pushFront(3);
    check(list.size() == 3, "Expected size 3 after pushFront");
    check(list.at(0) == 3, "Expected first element 3");
    check(list.at(2) == 1, "Expected last element 1");
}

TEST(test_push_front_updates_size)
{
    LinkedList list;
    list.pushFront(10);
    check(list.size() == 1, "Expected size 1 after single pushFront");
    check(!list.empty(), "List should not be empty after pushFront");
}

TEST(test_remove_head)
{
    LinkedList list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    // Removing head node (value 1) should work
    check(list.remove(1), "Should successfully remove head element");
    check(list.size() == 2, "Expected size 2 after removing head");
    check(list.at(0) == 2, "Expected new head to be 2");
}

TEST(test_remove_middle)
{
    LinkedList list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    check(list.remove(2), "Should successfully remove middle element");
    check(list.size() == 2, "Expected size 2 after remove");
    auto vec = list.toVector();
    check(vec[0] == 1 && vec[1] == 3, "Expected [1, 3]");
}

TEST(test_remove_nonexistent)
{
    LinkedList list;
    list.pushBack(1);
    check(!list.remove(99), "Should return false for nonexistent element");
}

TEST(test_contains)
{
    LinkedList list;
    list.pushBack(10);
    list.pushBack(20);
    check(list.contains(10), "Should contain 10");
    check(list.contains(20), "Should contain 20");
    check(!list.contains(30), "Should not contain 30");
}

TEST(test_empty_list)
{
    LinkedList list;
    check(list.empty(), "New list should be empty");
    check(list.size() == 0, "New list size should be 0");
}

TEST(test_clear)
{
    LinkedList list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    list.clear();
    check(list.empty(), "List should be empty after clear");
    check(list.size() == 0, "Size should be 0 after clear");
}

TEST(test_to_vector)
{
    LinkedList list;
    list.pushBack(5);
    list.pushBack(10);
    list.pushBack(15);
    auto vec = list.toVector();
    check(vec.size() == 3, "Vector should have 3 elements");
    check(vec[0] == 5 && vec[1] == 10 && vec[2] == 15, "Vector should be [5, 10, 15]");
}

TEST(test_mixed_push)
{
    LinkedList list;
    list.pushBack(2);
    list.pushFront(1);
    list.pushBack(3);
    check(list.size() == 3, "Mixed push should give size 3");
    auto vec = list.toVector();
    check(vec[0] == 1 && vec[1] == 2 && vec[2] == 3, "Expected [1, 2, 3]");
}

int main()
{
    std::cout << "\nLinked List Tests:\n";
    std::cout << "\n" << testsPassed << "/" << testsRun << " tests passed.\n";

    return (testsPassed == testsRun) ? 0 : 1;
}
