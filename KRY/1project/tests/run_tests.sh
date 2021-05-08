#!/bin/bash 

#-------------------------------------#
#----- FIT VUT ------ Tomáš Ryšavý ---#
#--- projekt KRY ------ xrysav27 -----#
#----- Vigener ------ 02. 04. 2021 ---#
#-------------------------------------#

# Sript will perform all the test in 
# this folder.
# To make this script working the folder 
# structure must look like this:

# project_folder/
# ├── tests/
# │   ├── run_tests.sh
# │   ├── test1.in
# │   ├── test1.1.out
# │   ├── test1.2.out
# │   ├── test2.in
# │   ├── test2.1.out
# │   ├── test2.2.out
# │   └── •••
# └── kry

GREEN='\033[1;32m'
CYAN='\033[0;36m'
RED='\033[1;31m'
YELLOW='\033[0;33m'
NC='\033[0;m'

TESTCOUNT=`ls -l ./*.in | wc -l`
TEMPFILE="test.temp"
TESTVARIANTS=(1 2)
# percent of friedman tolerance in decimal number
FRIEDMANTOLERANCE="0.2"

PASSEDTESTS=0
FAILEDTESTS=0
for TESTNO in $(seq 1 ${TESTCOUNT}); do
	FILE="test${TESTNO}.in"
	FILEOUT="test${TESTNO}.out"
    echo -e "\n${GREEN}### TEST No. ${TESTNO} ###"
    echo -e "${NC}# Script output: ../kry < ${FILE}${RED}"
    `../kry < ${FILE} > ${TEMPFILE}`

    # LOAD FILES
    ###########
    printf -v ESC_TEMP_FILE "%q\n" ${TEMPFILE}
  	IFS=$'\r\n' GLOBIGNORE='*' command eval  'ESC_TEMP_FILE_ARRAY=($(cat ${ESC_TEMP_FILE}))'

    printf -v ESC_FILE_OUT "%q\n" ${FILEOUT}
  	IFS=$'\r\n' GLOBIGNORE='*' command eval  'ESC_FILE_OUT_ARRAY=($(cat ${ESC_FILE_OUT}))'

    # CHECK NONTERMS
    IFS=$';' read -a TEMP_FILE_ARRAY <<< "${ESC_TEMP_FILE_ARRAY[0]}"

    IFS=$';' read -a FILE_OUT_ARRAY <<< "${ESC_FILE_OUT_ARRAY[0]}"

	DIF_FRIEDMAN=`echo "${TEMP_FILE_ARRAY[0]} > ${FILE_OUT_ARRAY[0]}-${FILE_OUT_ARRAY[0]}*${FRIEDMANTOLERANCE} && ${TEMP_FILE_ARRAY[0]} < ${FILE_OUT_ARRAY[0]}+${FILE_OUT_ARRAY[0]}*${FRIEDMANTOLERANCE} " | bc -l`
	

	DIF_KASISKI=`diff <(echo ${TEMP_FILE_ARRAY[1]}) <(echo ${FILE_OUT_ARRAY[1]})`
	
	DIF_KEY_LENGTH=`diff <(echo ${TEMP_FILE_ARRAY[2]}) <(echo ${FILE_OUT_ARRAY[2]})`
	
	DIF_KEY=`diff <(echo ${TEMP_FILE_ARRAY[3]}) <(echo ${FILE_OUT_ARRAY[3]})`


    PRINT_IF_AS_EXPECTED=true # true
    ONE="1"
    ZERO="0"
    # RESULTS
	#if [ -z "$DIF_NONTERMS" ] && [ -z "$DIF_TERMS" ] && [ -z "$DIF_START" ] && [ -z "$DIF_RULES" ]; then
	if [ $DIF_FRIEDMAN -eq $ONE ] && [ -z "$DIF_KASISKI" ] && [ -z "$DIF_KEY_LENGTH" ] && [ -z "$DIF_KEY" ]; then

		if [ "$PRINT_IF_AS_EXPECTED" == "true" ]; then
			echo -e "${NC}TEST CASE"
			echo "-----------------"
		    	printf "%s " "${ESC_FILE_OUT_ARRAY}"
		    	echo ""
		    	printf "%s " "${ESC_TEMP_FILE_ARRAY}"
			echo ""
			echo "-----------------"
	    	fi

		echo -e "${NC}Result:$GREEN OK $NC"
		PASSEDTESTS=$((PASSEDTESTS+1))
	else
		echo -e "${NC}EXPECTED"
		echo "-----------------"
    		printf "%s " "${ESC_FILE_OUT_ARRAY}"

		echo ""
		echo "YOUR OUTPUT"
		echo "-----------------"
    		printf "%s " "${ESC_TEMP_FILE_ARRAY}"
		echo ""
		echo "-----------------"
		
		FAILEDTESTS=$((FAILEDTESTS+1))
		
		if [ $DIF_FRIEDMAN -eq $ZERO ] && [ -z "$DIF_KASISKI" ] && [ -z "$DIF_KEY_LENGTH" ] && [ -z "$DIF_KEY" ]; then
			echo -e "Result:$RED DIFF NOT PASSED BECAUSE OF FRIEDMAN.$NC"
			FRIEDMANFALSE=$((FRIEDMANFALSE+1))
		else
			echo -e "Result:$RED DIFF NOT PASSED.$NC"
		fi
	fi
    echo -e "${CYAN}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~${NC}"
done
echo -e "FINAL RESULTS: $GREEN PASSED: $PASSEDTESTS $RED FAILED: ${FAILEDTESTS} $YELLOW (BECAUSE OF FRIEDMAN: ${FRIEDMANFALSE})$NC"
#`rm test.temp`
