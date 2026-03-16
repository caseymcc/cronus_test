#include "testFramework.h"
#include "item.h"

#include <sstream>

// ---- Item struct tests ----

TEST(ItemEquality)
{
    Item a{"Widget", "Hardware", 10, 5.99};
    Item b{"Widget", "Hardware", 10, 5.99};
    ASSERT_TRUE(a == b);
}

TEST(ItemInequality)
{
    Item a{"Widget", "Hardware", 10, 5.99};
    Item b{"Gadget", "Hardware", 10, 5.99};
    ASSERT_TRUE(a != b);
}

TEST(ItemInequalityQuantity)
{
    Item a{"Widget", "Hardware", 10, 5.99};
    Item b{"Widget", "Hardware", 20, 5.99};
    ASSERT_TRUE(a != b);
}

TEST(ItemToString)
{
    Item item{"Widget", "Hardware", 10, 5.99};
    std::string str=item.toString();
    ASSERT_TRUE(str.find("Widget") != std::string::npos);
    ASSERT_TRUE(str.find("Hardware") != std::string::npos);
    ASSERT_TRUE(str.find("10") != std::string::npos);
}

TEST(ItemDefaultValues)
{
    Item item;
    ASSERT_EQ(item.name, std::string(""));
    ASSERT_EQ(item.category, std::string(""));
    ASSERT_EQ(item.quantity, 0);
    ASSERT_NEAR(item.price, 0.0, 0.001);
}
