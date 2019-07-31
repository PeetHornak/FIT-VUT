#include <stdio.h>
#include <stdlib.h>

#include "../src/header/scanner.h"
#include "../src/header/parseexp.h"
#include "../src/header/garbagecollector.h"
#include "../src/header/symtable.h"
#include "../src/header/instrlist.h"

int main()
{
    garbage_collector_init();
    htab_setup();
    myQueueInit();
    tDLList *l = gb_malloc(sizeof(tDLList));
    DLInsertFirst(l,".IFJcode18\n");
    DLInsertLast(l,"CREATEFRAME\nPUSHFRAME\n");
    DLInsertLast(l,"DEFVAR LF@$vysl\nPOPS LF@$vysl\nWRITE LF@$vysl\n"
                    "WRITE string@\\010\nPOPFRAME");
    DLLast(l);

    if(!expression_parse( TYPE_EOL,NULL ,NULL,l))
        puts("Syntax not correct!!!");
    /*
    if(expression_parse( TYPE_EOL,NULL ,htab_return_pointer(),l))
        puts("Syntax correct!!!");
    else
        puts("Syntax not correct!!!");
    */

    myQueueFree();

    DLPrintList(l);
    gb_release_resources();
    return 0;
}
