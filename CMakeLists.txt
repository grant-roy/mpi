cmake_minimum_required(VERSION 3.2)
project(mpi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES calculate_pi.c)
add_executable(mpi ${SOURCE_FILES})