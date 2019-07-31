cmake_minimum_required(VERSION 3.13.0)

add_executable(queueTests TestsMain.cpp tests.cpp catch.hpp)

target_link_libraries(queueTests queue::queue)
