#!/bin/sh

files=$(ls *.src)
counter=1
folder=$(pwd | rev | cut -d '/' -f 1 | rev )

echo -e "\n\n ====== $folder ====== \n\n"

for file in $files; do
    ../../../ifj18 < $file > $file.code
    echo "$file - status [ $? ]"
done


echo -e "\n\n ===================== "
