#!/bin/sh

mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=DEBUG -D SDL2_DIR=/mnt/matylda1/imilet/installedLibs/lib/cmake/SDL2 ..
make
