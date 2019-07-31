#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "header/parser.h"
#include "header/symtable.h"
#include "header/scanner.h"
#include "header/generate.h"

int main(){
    
    htab_setup();
    myQueueInit();
    myStackInit();
    bool corr = true;
    corr =  parse();
    gb_release_resources();
    if(corr == true)
        return 0;
}
