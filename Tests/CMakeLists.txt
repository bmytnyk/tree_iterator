cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

include_directories(Include ${JNI_DIRS} googletest/include)

set(BUILD_GTEST 1)

add_subdirectory(googletest)

set(TREE_HEADER_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories(${TREE_HEADER_DIRECTORY})

set(TREE_HEADERS 
	"${TREE_HEADER_DIRECTORY}/tree.h"
	"${TREE_HEADER_DIRECTORY}/tree_item.h"
	"${TREE_HEADER_DIRECTORY}/bfs_adaptor.h"
	"${TREE_HEADER_DIRECTORY}/dfs_adaptor.h"
	"${TREE_HEADER_DIRECTORY}/optimized_ptr_stack.h")

if (MSVC)
	SET( CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} /MTd" CACHE STRING "MSVC MT flags " FORCE)
	SET( CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} /MT" CACHE STRING "MSVC MT flags " FORCE)
endif (MSVC)

add_executable(test_correctness test_correctness.cpp test_move_copy.cpp ${TREE_HEADERS})

target_link_libraries(test_correctness gtest)

add_executable(test_performance test_performance.cpp ${TREE_HEADERS})

target_link_libraries(test_performance gtest)
