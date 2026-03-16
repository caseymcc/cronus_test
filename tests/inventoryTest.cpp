#include "testFramework.h"
#include "inventory.h"

#include <sstream>

// ---- Basic operations (these pass) ----

TEST(AddItem)
{
    Inventory inv;
    Item item{"Widget", "Hardware", 10, 5.99};
    ASSERT_TRUE(inv.addItem(item));
    ASSERT_EQ(inv.size(), size_t(1));
}

TEST(AddItemEmptyName)
{
    Inventory inv;
    Item item{"", "Hardware", 10, 5.99};
    ASSERT_FALSE(inv.addItem(item));
    ASSERT_EQ(inv.size(), size_t(0));
}

TEST(AddDuplicateMerges)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    inv.addItem({"Widget", "Hardware", 5, 5.99});
    ASSERT_EQ(inv.size(), size_t(1));

    auto item=inv.findItem("Widget");
    ASSERT_TRUE(item.has_value());
    ASSERT_EQ(item->quantity, 15);
}

TEST(FindItemCaseInsensitive)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});

    auto result=inv.findItem("widget");
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->name, std::string("Widget"));
}

TEST(FindItemNotFound)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    ASSERT_FALSE(inv.findItem("Gadget").has_value());
}

TEST(UpdateQuantity)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    ASSERT_TRUE(inv.updateQuantity("Widget", 25));

    auto item=inv.findItem("Widget");
    ASSERT_EQ(item->quantity, 25);
}

TEST(UpdateQuantityNegativeFails)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    ASSERT_FALSE(inv.updateQuantity("Widget", -5));
}

TEST(GetAllItems)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    inv.addItem({"Gadget", "Electronics", 5, 29.99});

    auto items=inv.getAllItems();
    ASSERT_EQ(items.size(), size_t(2));
}

TEST(ClearInventory)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    inv.addItem({"Gadget", "Electronics", 5, 29.99});
    inv.clear();
    ASSERT_TRUE(inv.empty());
    ASSERT_EQ(inv.getTotalItemCount(), 0);
}

TEST(TotalValue)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.00});
    inv.addItem({"Gadget", "Electronics", 5, 20.00});
    // 10*5 + 5*20 = 50 + 100 = 150
    ASSERT_NEAR(inv.getTotalValue(), 150.0, 0.01);
}

// ---- BUG: removeItem doesn't update total count ----
// After removing an item, getTotalItemCount() should reflect the removal.
// This test FAILS because removeItem forgets to subtract the quantity.

TEST(RemoveItemUpdatesTotalCount)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    inv.addItem({"Gadget", "Electronics", 5, 29.99});
    ASSERT_EQ(inv.getTotalItemCount(), 15);

    inv.removeItem("Widget");
    ASSERT_EQ(inv.size(), size_t(1));
    ASSERT_EQ(inv.getTotalItemCount(), 5);  // FAILS: returns 15 instead of 5
}

TEST(RemoveItemTotalValueUpdated)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.00});
    inv.addItem({"Gadget", "Electronics", 5, 20.00});
    ASSERT_NEAR(inv.getTotalValue(), 150.0, 0.01);

    inv.removeItem("Widget");
    ASSERT_NEAR(inv.getTotalValue(), 100.0, 0.01);
    ASSERT_EQ(inv.getTotalItemCount(), 5);  // FAILS: returns 15 instead of 5
}

// ---- BUG: findItemsByCategory is case-sensitive ----
// Searching for "electronics" should find items with category "Electronics".
// This test FAILS because the comparison doesn't use toLower().

TEST(FindByCategoryCaseInsensitive)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    inv.addItem({"Laptop", "Electronics", 3, 999.99});
    inv.addItem({"Phone", "Electronics", 8, 599.99});

    auto results=inv.findItemsByCategory("electronics");
    ASSERT_EQ(results.size(), size_t(2));  // FAILS: returns 0 (case mismatch)
}

TEST(FindByCategoryExactCase)
{
    Inventory inv;
    inv.addItem({"Widget", "Hardware", 10, 5.99});
    inv.addItem({"Laptop", "Electronics", 3, 999.99});

    // Exact case should always work
    auto results=inv.findItemsByCategory("Electronics");
    ASSERT_EQ(results.size(), size_t(1));
}

// ---- FEATURE: getLowStockItems (not implemented yet) ----
// These tests will FAIL at compile time until the feature is added.
// Uncomment them when testing the "low stock" feature task.

// TEST(GetLowStockItems)
// {
//     Inventory inv;
//     inv.addItem({"Widget", "Hardware", 10, 5.99});
//     inv.addItem({"Gadget", "Electronics", 2, 29.99});
//     inv.addItem({"Cable", "Electronics", 1, 3.99});
//
//     auto lowStock=inv.getLowStockItems(5);
//     ASSERT_EQ(lowStock.size(), size_t(2));
// }
//
// TEST(GetLowStockItemsNone)
// {
//     Inventory inv;
//     inv.addItem({"Widget", "Hardware", 10, 5.99});
//     inv.addItem({"Gadget", "Electronics", 20, 29.99});
//
//     auto lowStock=inv.getLowStockItems(5);
//     ASSERT_EQ(lowStock.size(), size_t(0));
// }
//
// TEST(GetLowStockItemsEmpty)
// {
//     Inventory inv;
//     auto lowStock=inv.getLowStockItems(5);
//     ASSERT_EQ(lowStock.size(), size_t(0));
// }

// ---- FEATURE: JSON serialization (not implemented yet) ----
// These tests will FAIL at compile time until the feature is added.
// Uncomment them when testing the "JSON persistence" feature task.

// TEST(ToJson)
// {
//     Inventory inv;
//     inv.addItem({"Widget", "Hardware", 10, 5.99});
//     inv.addItem({"Gadget", "Electronics", 5, 29.99});
//
//     std::string json=inv.toJson();
//     ASSERT_TRUE(json.find("Widget") != std::string::npos);
//     ASSERT_TRUE(json.find("Gadget") != std::string::npos);
//     ASSERT_TRUE(json.find("Hardware") != std::string::npos);
// }
//
// TEST(FromJson)
// {
//     Inventory original;
//     original.addItem({"Widget", "Hardware", 10, 5.99});
//     original.addItem({"Gadget", "Electronics", 5, 29.99});
//
//     std::string json=original.toJson();
//
//     Inventory loaded;
//     ASSERT_TRUE(loaded.fromJson(json));
//     ASSERT_EQ(loaded.size(), size_t(2));
//     ASSERT_EQ(loaded.getTotalItemCount(), 15);
//
//     auto widget=loaded.findItem("Widget");
//     ASSERT_TRUE(widget.has_value());
//     ASSERT_EQ(widget->quantity, 10);
//     ASSERT_NEAR(widget->price, 5.99, 0.01);
// }
//
// TEST(FromJsonEmpty)
// {
//     Inventory inv;
//     ASSERT_TRUE(inv.fromJson("[]"));
//     ASSERT_EQ(inv.size(), size_t(0));
// }
//
// TEST(FromJsonInvalid)
// {
//     Inventory inv;
//     ASSERT_FALSE(inv.fromJson("not valid json"));
// }
