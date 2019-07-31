cmake_minimum_required(VERSION 3.13.0)

add_executable(stackTests TestsMain.cpp tests.cpp catch.hpp)

target_link_libraries(stackTests stack::stack)
