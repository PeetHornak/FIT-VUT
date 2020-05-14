#!/bin/bash

echo $1 > numbers
var=$1
max=25
res="${var//[^,]}"
number="${#res}"
number=$((number+2))
number=$((number/2))
number=$(( number > max ? max : number ))
mpic++ -o vid vid.cpp && mpirun -np $number ./vid < numbers
rm -f numbers
rm -f main
