#!/bin/bash
 
#Eyyy, this script can be used for testing IZPproj1 @VUTFIT2017
#you can do arbitrary edits in this file. Enjoy hacking!
 
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
 
success=true
 
echo "Testing proj1.c file existence..."
if [ ! -f ./proj1.c ]; then
        echo "File proj1.c not found!"
    echo -e "${RED}[FAILED]${NC}"
    success=false
    exit 1
else
    echo -e "${GREEN}[OK]${NC}"
fi
echo
 
echo "Looking into proj1.c for forbidden functions..."                                  #i should use regex, i know, i know...shhh
# bad luck if you use these words in comments
if grep -q "malloc" proj1.c  || grep -q "fopen" proj1.c || grep -q "fscanf" proj1.c || grep -q "qsort" proj1.c || grep -q "lsearch" proj1.c || grep -q "bsearch" proj1.c || grep -q "hsearch" proj1.c; then
    echo -e "${RED}[FAILED]${NC}"
    success=false
else
    echo -e "${GREEN}[OK]${NC}"
fi
echo
 
echo "Cleaning folder..."
rm -fv proj1
echo
 
echo "Trying to build proj1..."
make -s
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
    exit 1
fi
echo
 
 
echo "Creating DB..."
rm cities.txt
touch cities.txt
echo "Prcice" >> cities.txt
echo "Praha" >> cities.txt
echo "Prachatice" >> cities.txt
echo "Brno" >> cities.txt
echo "Bratislava" >> cities.txt
echo "brankovice" >> cities.txt
echo "liberec" >> cities.txt
echo "LOuny" >> cities.txt
echo "Adamov" >> cities.txt
echo "As" >> cities.txt
echo "ASASAS" >> cities.txt
echo "Teplice" >> cities.txt
echo "Olomouc" >> cities.txt
echo "Zilina je krasna" >> cities.txt
echo "zilina je hezka" >> cities.txt
 
echo -e "${GREEN}[OK]${NC}"
echo
 
echo "Running without parameter..."
echo "Expected output: \"Enable: ABLOPTZ\""
OUTPUT="$(./proj1 < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: ABLOPTZ" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"a\"..."
echo "Expected output: \"Enable: DS\""
 
OUTPUT="$(./proj1 a < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: DS" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"praha-mesto\" (testing parameter longer than word)..."
echo "Expected output: \"Not found\""
 
OUTPUT="$(./proj1 praha-mesto < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Not found" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"pr\" (testing if output is sorted and each char is present just once)"
echo "Expected output: \"Enable: AC\""
 
OUTPUT="$(./proj1 pr < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: AC" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"PR\" (testing case insensivity)"
echo "Expected output: \"Enable: AC\""
 
OUTPUT="$(./proj1 PR < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: AC" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"l\" (testing case insencivity)"
echo "Expected output: \"Enable: IO\""
 
OUTPUT="$(./proj1 l < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: IO" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
# this test may be wrong. Output should be all caps (OLOMOUC).
echo "Running with parameter \"o\" (testing single appearance of first letter)"
echo "Expected output: \"Found: OLOMOUC\""
 
OUTPUT="$(./proj1 o < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Found: OLOMOUC" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"bran\" (testing single appearance of prefix)"
echo "Expected output: \"Found: BRANKOVICE\""
 
OUTPUT="$(./proj1 bran < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Found: BRANKOVICE" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"f\" (testing no existence of first letter)"
echo "Expected output: \"Not found\""
 
OUTPUT="$(./proj1 f < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Not found" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
 
echo "Running with parameter \"zilina\" (testing space in city name)"
echo "Expected output: \"Enable:  \""
 
OUTPUT="$(./proj1 'zilina' < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable:  " ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
 
echo "Running with parameter \"zilina \" (testing space in city name)"
echo "Expected output: \"Enable: J\""
 
OUTPUT="$(./proj1 'zilina ' < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: J" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
 
echo "Running with parameter \"zilina j\" (testing space in city name)"
echo "Expected output: \"Enable: E\""
 
OUTPUT="$(./proj1 'zilina j' < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: E" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"zilina je\" (testing space in city name)"
echo "Expected output: \"Enable:  \""
 
OUTPUT="$(./proj1 'zilina je' < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable:  " ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
echo "Running with parameter \"zilina je \" (testing space in city name)"
echo "Expected output: \"Enable: HK\""
 
OUTPUT="$(./proj1 'zilina je ' < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: HK" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo
 
 
echo "Not Running with parameter \"as\" (testing city name is prefix of another city name, idk what should happen)"
echo "Expected output: not defined"
 
#OUTPUT="$(./proj1 as < cities.txt)"
#if [ "$OUTPUT" == "Not found" ]; then
#   echo -e "${GREEN}[OK]${NC}"
#else
#   echo -e "${RED}[FAILED]${NC}"
#fi
echo

echo "XmestoSpresne100znakyVnazvuabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789abcdefgijklm" >> cities.txt
echo "Running without parameter (testing city name len == 100)"
echo "Expected output: \"Enable: ABLOPTXZ\""
 
OUTPUT="$(./proj1 < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: ABLOPTXZ" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo

echo "YmestoSviceNEZ100znakyVnazvuabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789abcdefgijklm" >> cities.txt
echo "Running without parameter (testing city name len > 100)"
echo "Expected output: Enable: ABLOPTXYZ"
 
OUTPUT="$(./proj1 < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Enable: ABLOPTXYZ" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo


# could anybody doublecheck this?
echo "Running without parameter (testing city name len > 100, cutting name)"
echo "Expected output: Found: YMESTOSVICENEZ100ZNAKYVNAZVUABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ123456789ABCDEFGIJKL"
 
OUTPUT="$(./proj1 Y < cities.txt)"
echo -n "Your output: "
echo "\"$OUTPUT\""
if [ "$OUTPUT" == "Found: YMESTOSVICENEZ100ZNAKYVNAZVUABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ123456789ABCDEFGIJKL" ]; then
    echo -e "${GREEN}[OK]${NC}"
else
    echo -e "${RED}[FAILED]${NC}"
    success=false
fi
echo


if [ "$success" = true ] ; then
    echo -e "${GREEN}All tests [OK]${NC}"
    echo "Good job"
else
    echo -e "${RED}Some test [FAILED]${NC}"
    echo "Fix your code!"
fi

