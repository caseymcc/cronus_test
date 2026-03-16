cmake_minimum_required(VERSION 3.16)
project(inventory_manager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(inventory
    src/inventory.cpp
    src/item.cpp
)
target_include_directories(inventory PUBLIC include)

# Main executable
add_executable(inventory_cli src/main.cpp)
target_link_libraries(inventory_cli PRIVATE inventory)

# Tests
enable_testing()
add_executable(inventory_tests
    tests/testMain.cpp
    tests/inventoryTest.cpp
    tests/itemTest.cpp
)
target_link_libraries(inventory_tests PRIVATE inventory)
add_test(NAME inventory_tests COMMAND inventory_tests)
