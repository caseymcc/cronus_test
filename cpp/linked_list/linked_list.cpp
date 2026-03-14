#include "linked_list.h"
#include <stdexcept>
#include <sstream>

LinkedList::LinkedList()
    : m_head(nullptr), m_size(0)
{
}

LinkedList::~LinkedList()
{
    // BUG: Only frees the head node, leaks the rest
    if(m_head)
    {
        delete m_head;
    }
}

void LinkedList::pushBack(int value)
{
    Node *newNode = new Node(value);

    if(!m_head)
    {
        m_head = newNode;
    }
    else
    {
        Node *current = m_head;

        while(current->next)
        {
            current = current->next;
        }

        current->next = newNode;
    }

    m_size++;
}

void LinkedList::pushFront(int value)
{
    Node *newNode = new Node(value);
    newNode->next = m_head;
    m_head = newNode;
    // BUG: m_size not incremented
}

bool LinkedList::remove(int value)
{
    if(!m_head)
    {
        return false;
    }

    // BUG: Skips checking the head node, starts at second element
    Node *prev = m_head;
    Node *current = m_head->next;

    while(current)
    {
        if(current->data == value)
        {
            prev->next = current->next;
            delete current;
            m_size--;
            return true;
        }

        prev = current;
        current = current->next;
    }

    return false;
}

bool LinkedList::contains(int value) const
{
    Node *current = m_head;

    while(current)
    {
        if(current->data == value)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

int LinkedList::at(size_t index) const
{
    if(index >= m_size)
    {
        throw std::out_of_range("Index out of range");
    }

    Node *current = m_head;

    for(size_t i = 0; i < index; i++)
    {
        current = current->next;
    }

    return current->data;
}

size_t LinkedList::size() const
{
    return m_size;
}

bool LinkedList::empty() const
{
    return m_size == 0;
}

void LinkedList::clear()
{
    Node *current = m_head;

    while(current)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }

    m_head = nullptr;
    m_size = 0;
}

std::vector<int> LinkedList::toVector() const
{
    std::vector<int> result;
    Node *current = m_head;

    while(current)
    {
        result.push_back(current->data);
        current = current->next;
    }

    return result;
}

std::string LinkedList::toString() const
{
    std::ostringstream oss;
    oss << "[";
    Node *current = m_head;

    while(current)
    {
        oss << current->data;

        if(current->next)
        {
            oss << " -> ";
        }

        current = current->next;
    }

    oss << "]";
    return oss.str();
}
