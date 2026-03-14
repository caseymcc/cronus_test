#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <cstddef>
#include <string>
#include <vector>

/**
 * A simple singly-linked list of integers.
 *
 * BUGS:
 * - push_front doesn't update size
 * - remove() has an off-by-one that skips the head node
 * - destructor doesn't free all nodes (memory leak)
 */
class LinkedList
{
public:
    LinkedList();
    ~LinkedList();

    // Non-copyable for simplicity
    LinkedList(const LinkedList &) = delete;
    LinkedList &operator=(const LinkedList &) = delete;

    void pushBack(int value);
    void pushFront(int value);
    bool remove(int value);
    bool contains(int value) const;
    int at(size_t index) const;
    size_t size() const;
    bool empty() const;
    void clear();

    std::vector<int> toVector() const;
    std::string toString() const;

private:
    struct Node
    {
        int data;
        Node *next;

        Node(int val) : data(val), next(nullptr) {}
    };

    Node *m_head;
    size_t m_size;
};

#endif // LINKED_LIST_H
