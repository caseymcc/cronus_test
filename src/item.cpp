#include "item.h"

#include <sstream>

bool Item::operator==(const Item &other) const
{
    return name == other.name
        && category == other.category
        && quantity == other.quantity
        && price == other.price;
}

bool Item::operator!=(const Item &other) const
{
    return !(*this == other);
}

std::string Item::toString() const
{
    std::ostringstream oss;
    oss << name << " [" << category << "] qty=" << quantity
        << " price=" << price;
    return oss.str();
}
