#!/bin/sh


files=$(ls -d */)
counter=1
folder=$(pwd | rev | cut -d '/' -f 1 | rev )


for file in $files; do
    cd $file/testoutput
    sh testinterpret.sh
    cd ../../
done