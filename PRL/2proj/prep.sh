#!/bin/bash

var=$1
max=25
res="${var//[^,]}"
number="${#res}"
echo $number
number=$((number+2))
echo $number
number=$((number/2))
echo $number
number=$(( number > max ? max : number ))
echo $number
