cmake_minimum_required(VERSION 3.14)
project(AVLTree)

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(range_queries src/main.cc)

target_include_directories(
    range_queries
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(CTest)

file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/*.dat")

foreach(testfile ${TEST_FILES})
    get_filename_component(testname ${testfile} NAME_WE)

    set(answer_file_range_queries "${CMAKE_CURRENT_SOURCE_DIR}/tests/answ/${testname}.ans")

    add_test(
        NAME range_queries_${testname}
        COMMAND /bin/sh -c "./range_queries < ${testfile} | diff -u - ${answer_file_range_queries}"
    )

endforeach()

add_subdirectory(tests)

