// Minimal test framework - no external dependencies
#ifndef _TEST_FRAMEWORK_H_
#define _TEST_FRAMEWORK_H_

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cmath>
#include <sstream>
#include <stdexcept>

struct TestEntry {
    std::string name;
    std::function<void()> func;
};

// Shared test registry (defined in testMain.cpp)
std::vector<TestEntry> &getTests();

struct TestRegistrar {
    TestRegistrar(const char *name, std::function<void()> func)
    {
        getTests().push_back({name, std::move(func)});
    }
};

#define TEST(name) \
    void test_##name(); \
    static TestRegistrar registrar_##name(#name, test_##name); \
    void test_##name()

#define ASSERT_TRUE(expr) \
    do { \
        if(!(expr)) { \
            throw std::runtime_error( \
                std::string("ASSERT_TRUE failed: ") + #expr \
                + " at " + __FILE__ + ":" + std::to_string(__LINE__)); \
        } \
    } while(0)

#define ASSERT_FALSE(expr) \
    do { \
        if(expr) { \
            throw std::runtime_error( \
                std::string("ASSERT_FALSE failed: ") + #expr \
                + " at " + __FILE__ + ":" + std::to_string(__LINE__)); \
        } \
    } while(0)

#define ASSERT_EQ(a, b) \
    do { \
        auto _a=(a); auto _b=(b); \
        if(_a != _b) { \
            std::ostringstream _oss; \
            _oss << "ASSERT_EQ failed: " << #a << " == " << #b \
                 << " (" << _a << " != " << _b << ")" \
                 << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(_oss.str()); \
        } \
    } while(0)

#define ASSERT_NEAR(a, b, eps) \
    do { \
        auto _a=(a); auto _b=(b); \
        if(std::fabs(_a - _b) > (eps)) { \
            std::ostringstream _oss; \
            _oss << "ASSERT_NEAR failed: " << #a << " ~= " << #b \
                 << " (" << _a << " vs " << _b << ", eps=" << (eps) << ")" \
                 << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(_oss.str()); \
        } \
    } while(0)

inline int runAllTests(const std::string &suite)
{
    auto &tests=getTests();
    std::cout << "=== " << suite << " ===" << std::endl;

    int passed=0;
    int failed=0;

    for(const auto &test : tests)
    {
        try
        {
            test.func();
            std::cout << "  PASS: " << test.name << std::endl;
            passed++;
        }
        catch(const std::exception &e)
        {
            std::cout << "  FAIL: " << test.name << std::endl;
            std::cout << "        " << e.what() << std::endl;
            failed++;
        }
    }

    std::cout << "\n" << suite << ": " << passed << " passed, "
              << failed << " failed, " << (passed + failed)
              << " total" << std::endl;

    return failed > 0 ? 1 : 0;
}

#endif//_TEST_FRAMEWORK_H_
