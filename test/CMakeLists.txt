cmake_minimum_required(VERSION 3.22)
project(test)

set(TEST_SOURCES test.cpp)

# Construct test executable
add_executable(kino.test ${TEST_SOURCES})
target_link_libraries(
  kino.test
  GTest::gtest
  GTest::gmock
  GTest::gtest_main
  GTest::gmock_main
  kino)

add_test(NAME kino.test COMMAND kino.test)