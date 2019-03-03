#!/bin/bash

# testuje spravnou sekvenci cislovani akci a format vystupu (pouze syntax)
# bez zaruky, muze obsahovat chyby

outf=$1

# odstraneni bilych znaku
tr -d " \t" < ${outf} > $$

# test cislovani akci
# tiskne radky s chybnym cislovanim
cat $$ | awk -F":" ' { c++; if (c != $1) print NR, " => ", $1, " : chyba v cislovani akci"; } '

declare -a lines
lines[0]='^[1-9][0-9]*:RID[1-9][0-9]*:start$'
lines[1]='^[1-9][0-9]*:RID[1-9][0-9]*:enter:[0-9][0-9]*$'
lines[2]='^[1-9][0-9]*:RID[1-9][0-9]*:boarding$'
lines[3]='^[1-9][0-9]*:RID[1-9][0-9]*:finish$'
lines[4]='^[1-9][0-9]*:BUS:start$'
lines[5]='^[1-9][0-9]*:BUS:arrival$'
lines[6]='^[1-9][0-9]*:BUS:depart$'
lines[7]='^[1-9][0-9]*:BUS:end$'
lines[8]='^[1-9][0-9]*:BUS:startboarding:[0-9][0-9]*$'
lines[9]='^[1-9][0-9]*:BUS:endboarding:[0-9][0-9]*$'
lines[10]='^[1-9][0-9]*:BUS:finish$'

# kontrola sytaxe vystupu
# vytiskne radky, ktere neodpovidaji formatu vytupu
echo "=== radky, ktere neodpovidaji formatu vystupu"

for i in `seq 0 10`
do
    echo "/${lines[$i]}/d" >> $$-sed
done

sed -f $$-sed $$

# kontrola chybejicich vystupu
# tiskne informaci, ktery text ve vystupu chybi
echo "=== chybejici vystupy"
#echo ">> chybejici waiting nemusi nutne znamenat chybu, za urcitych okolnosti nemusi ve vystupu byt <<"

for i in `seq 0 12`
do
    cat $$ | grep "${lines[$i]}" >/dev/null || echo "${lines[$i]}"
done


rm $$*
