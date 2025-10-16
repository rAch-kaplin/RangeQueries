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
