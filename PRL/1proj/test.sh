#!/bin/bash

dd if=/dev/random bs=1 count=$1 of=numbers 2> /dev/null

mpic++ -o ots ots.cpp && mpirun -np $1 ./ots

rm -f ots numbers
