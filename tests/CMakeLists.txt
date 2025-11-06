cmake_minimum_required(VERSION 3.16)

enable_testing()
find_package(GTest REQUIRED)

add_executable(range_queries_test
    test_avl_tree.cc
)

target_include_directories(range_queries_test PRIVATE ../include)
target_link_libraries(range_queries_test PRIVATE GTest::gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(range_queries_test)

