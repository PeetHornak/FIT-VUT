#!/bin/sh

files=$(ls *.code)
counter=1
folder=$(pwd | awk -F'/' '{print $(NF-1),"/",$NF}')

echo -e "\n ====== $folder ====== \n"

for file in $files; do
    if [ -s $file ];then
        echo "$file"
        ../../../../ifjcode18/ic18int $file
        echo -e "$file - status [ $? ]\n"
    else
        echo -e "$file - Was not compiled\n"
    fi
done


echo -e "\n ===================== \n"