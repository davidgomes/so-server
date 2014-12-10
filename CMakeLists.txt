project (so_server)
cmake_minimum_required (VERSION 2.8)

set (CMAKE_C_FLAGS "-Wall -pthread -g")

file (GLOB_RECURSE PROJECT_SOURCES "src/*.c")

include_directories (include)
add_executable (so-server ${PROJECT_SOURCES})
