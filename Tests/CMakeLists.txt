cmake_minimum_required(VERSION 3.5)

project(tree)

set (CMAKE_CXX_STANDARD 11)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
add_executable(test_iterator test.cpp)
