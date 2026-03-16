#include "testFramework.h"

std::vector<TestEntry> &getTests()
{
    static std::vector<TestEntry> tests;
    return tests;
}

int main()
{
    return runAllTests("Inventory Manager Tests");
}
