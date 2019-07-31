cmake_minimum_required(VERSION 3.13.0)

add_executable(internalTests 
  main.cpp 
  tests.cpp
  ../../student/cpu.h
  ../../student/cpu.c
  ../../student/gpu.h
  ../../student/gpu.c
  ../../student/linearAlgebra.h
  ../../student/linearAlgebra.c
  ../../student/drawPoints.cpp
  ../../student/drawTriangles.cpp
  ../../3rdParty/catch.hpp)

target_include_directories(internalTests PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
  )

#target_link_libraries(tests Vars::Vars)
target_link_libraries(internalTests stack::stack queue::queue vector::vector SDL2::SDL2 SDL2::SDL2main)
