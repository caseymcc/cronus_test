#ifndef _INVENTORY_ITEM_H_
#define _INVENTORY_ITEM_H_

#include <string>

struct Item {
    std::string name;
    std::string category;
    int quantity{0};
    double price{0.0};

    bool operator==(const Item &other) const;
    bool operator!=(const Item &other) const;

    std::string toString() const;
};

#endif//_INVENTORY_ITEM_H_
