cmake_minimum_required(VERSION 3.0)

#configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#  RESULT_VARIABLE result
#  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
#if(result)
#  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
#endif()

#execute_process(COMMAND ${CMAKE_COMMAND} --build .
#  RESULT_VARIABLE result
#  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

#if(result)
#  message(FATAL_ERROR "Build step for googletest failed: ${result}")
#endif()

set (CMAKE_CXX_STANDARD 11)

set(TREE_HEADER_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories(${TREE_HEADER_DIRECTORY})

set(TREE_HEADERS 
	"${TREE_HEADER_DIRECTORY}/tree.h"
	"${TREE_HEADER_DIRECTORY}/tree_item.h"
	"${TREE_HEADER_DIRECTORY}/bfs_adaptor.h"
	"${TREE_HEADER_DIRECTORY}/dfs_adaptor.h"
	"${TREE_HEADER_DIRECTORY}/optimized_ptr_stack.h")

add_executable(test_iterator test.cpp ${TREE_HEADERS})
