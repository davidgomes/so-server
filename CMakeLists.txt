cmake_minimum_required(VERSION 2.8.4)
project(so_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.c)

add_executable(so_server ${SOURCE_FILES})