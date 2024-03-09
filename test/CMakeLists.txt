cmake_minimum_required(VERSION 3.22)
project(unittest)

find_package(GTest REQUIRED)

set(TEST_SOURCES test.cpp)

# Construct test executable
add_executable(unittest ${TEST_SOURCES})
target_link_libraries(unittest gtest::gtest)
target_link_libraries(unittest booking)

add_test(NAME unittest COMMAND unittest)