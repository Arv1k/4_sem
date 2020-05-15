cmake_minimum_required(VERSION 3.15)
project(2)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

add_executable(2 main.cpp)

target_link_libraries (2 ${CMAKE_THREAD_LIBS_INIT})
