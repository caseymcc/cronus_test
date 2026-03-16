#ifndef _INVENTORY_INVENTORY_H_
#define _INVENTORY_INVENTORY_H_

#include "item.h"

#include <string>
#include <vector>
#include <optional>

class Inventory {
public:
    Inventory()=default;

    // Item management
    bool addItem(const Item &item);
    bool removeItem(const std::string &name);
    bool updateQuantity(const std::string &name, int newQuantity);

    // Queries
    std::optional<Item> findItem(const std::string &name) const;
    std::vector<Item> findItemsByCategory(const std::string &category) const;
    std::vector<Item> getAllItems() const;

    // Aggregates
    int getTotalItemCount() const;
    double getTotalValue() const;

    // Utility
    size_t size() const;
    bool empty() const;
    void clear();

private:
    std::vector<Item> m_items;
    int m_totalCount{0};

    // Inline string helpers (used for case-insensitive search)
    static std::string toLower(const std::string &str);
    static std::string trim(const std::string &str);
};

#endif//_INVENTORY_INVENTORY_H_
