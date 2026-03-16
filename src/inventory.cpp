#include "inventory.h"

#include <algorithm>
#include <cctype>

// ---- String helpers (used internally) ----

std::string Inventory::toLower(const std::string &str)
{
    std::string result=str;

    for(auto &ch : result)
    {
        ch=static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }

    return result;
}

std::string Inventory::trim(const std::string &str)
{
    size_t start=str.find_first_not_of(" \t\n\r");

    if(start == std::string::npos)
    {
        return "";
    }

    size_t end=str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

// ---- Item management ----

bool Inventory::addItem(const Item &item)
{
    std::string trimmedName=trim(item.name);

    if(trimmedName.empty())
    {
        return false;
    }

    // Check for duplicate (case-insensitive name match)
    std::string lowerName=toLower(trimmedName);

    for(auto &existing : m_items)
    {
        if(toLower(existing.name) == lowerName)
        {
            // Merge: add quantity to existing item
            int oldQty=existing.quantity;
            existing.quantity += item.quantity;
            m_totalCount += item.quantity;
            return true;
        }
    }

    Item newItem=item;
    newItem.name=trimmedName;
    m_items.push_back(newItem);
    m_totalCount += newItem.quantity;
    return true;
}

// BUG: removeItem erases the item but forgets to subtract its quantity
// from m_totalCount. After removal, getTotalItemCount() returns a value
// that is too high.
bool Inventory::removeItem(const std::string &name)
{
    std::string lowerName=toLower(trim(name));

    for(auto it=m_items.begin(); it != m_items.end(); ++it)
    {
        if(toLower(it->name) == lowerName)
        {
            m_items.erase(it);
            return true;
        }
    }

    return false;
}

bool Inventory::updateQuantity(const std::string &name, int newQuantity)
{
    if(newQuantity < 0)
    {
        return false;
    }

    std::string lowerName=toLower(trim(name));

    for(auto &item : m_items)
    {
        if(toLower(item.name) == lowerName)
        {
            int diff=newQuantity - item.quantity;
            item.quantity=newQuantity;
            m_totalCount += diff;
            return true;
        }
    }

    return false;
}

// ---- Queries ----

std::optional<Item> Inventory::findItem(const std::string &name) const
{
    std::string lowerName=toLower(trim(name));

    for(const auto &item : m_items)
    {
        if(toLower(item.name) == lowerName)
        {
            return item;
        }
    }

    return std::nullopt;
}

// BUG: findItemsByCategory compares categories directly without toLower(),
// so searching for "electronics" won't match items with category "Electronics".
std::vector<Item> Inventory::findItemsByCategory(const std::string &category) const
{
    std::vector<Item> results;
    std::string searchCat=trim(category);

    for(const auto &item : m_items)
    {
        if(item.category == searchCat)
        {
            results.push_back(item);
        }
    }

    return results;
}

std::vector<Item> Inventory::getAllItems() const
{
    return m_items;
}

// ---- Aggregates ----

int Inventory::getTotalItemCount() const
{
    return m_totalCount;
}

double Inventory::getTotalValue() const
{
    double total=0.0;

    for(const auto &item : m_items)
    {
        total += item.quantity * item.price;
    }

    return total;
}

// ---- Utility ----

size_t Inventory::size() const
{
    return m_items.size();
}

bool Inventory::empty() const
{
    return m_items.empty();
}

void Inventory::clear()
{
    m_items.clear();
    m_totalCount=0;
}
