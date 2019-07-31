#include <stdarg.h>

#include "header/generate.h"

/* Staticke premenne pre indexovanie labelov a premennych */
static int id_Param = 1;        // id volanych parametrov
static int id_if = 0;           // id labelov a pomocnych premennych pre if a while
static int id_defParam = 1;     // id parametov v definicii funkcie
static int id_dump = 1;         // id pre pomocnu premennu v pripade volania inputi/f/s bez priradenia
static int chr=0,substr=0,ord=0,length=0;
static tStack stack;            // stack na id pre while a if pri zanorení

void myStackInit(){
    SInit(&stack);
}

void addString(char **buffer, char *add)
{
    // Ak je pridavany string NULL nič nerobí
    if(add){
        if(*buffer){    // Ak string nie je NULL prida add nakoniec
            unsigned old = strlen(*buffer);
            unsigned new = old + strlen(add);
            *buffer = realloc(*buffer, new +1);
            strcat(*buffer,add);
            (*buffer)[new] = '\0';
        }
        else{           // Ak je string NULL vloží add
            unsigned size = strlen(add);
            *buffer = malloc(sizeof(char)*(size+1));
            strcpy(*buffer,add);
        }
    }

}

void fillString(char **buffer,char *formatted_string,...)
{
    // Praca s premennym poctom parametrov
    va_list l,l2;
    va_start(l,formatted_string);
    va_copy(l2,l);
    // Zistí dlžku na alokaciu
    unsigned size = vsnprintf(NULL,0,formatted_string,l);
    *buffer = malloc(size + sizeof(char));
    if(*buffer == NULL)
        gb_exit_process(99);
    // Vloží do bufferu
    vsnprintf(*buffer,size + sizeof(char),formatted_string,l2);
    va_end(l);
    va_end(l2);
}

void convertCharToString(char *array, char dumbchar)
{
    int dumbnumber = dumbchar;
    array[0]='\\';
    array[1]='0' + dumbnumber/100;
    array[2]='0' + dumbnumber/10 %10;
    array[3]='0' + dumbnumber%10;
}


void convertToString(char **buffer)
{
    int count_of_replaceable_char=0;
    // Zistí počet znakov ktore treba zmeniť
    for(unsigned i=0; i< strlen(*buffer);i++)
        if((*buffer)[i] < 33 || ((*buffer)[i] == 35) || (*buffer)[i] == 92)
            count_of_replaceable_char++;
    // Alokuje pamäť pre upraveny string
    char *formatedstring = malloc(strlen(*buffer)+count_of_replaceable_char*4 + 1);
    unsigned j=0;
    // Do formatedstringu pridava znaky ktore sa nemenia a konvertuje menene znaky
    for(unsigned i=0; i< strlen(*buffer);i++){
        if((*buffer)[i] < 33 || ((*buffer)[i] == 35) ||(*buffer)[i] == 92){
            convertCharToString(&formatedstring[j],(*buffer)[i]);
            j+=4;
        }
        else{
            formatedstring[j]=(*buffer)[i];
            j++;
        }
    }
    formatedstring[j]='\0';
    unsigned length= strlen(formatedstring)+1;
    // Reallocne buffer a vloží sformatovany string
    *buffer = realloc(*buffer,length);
    strcpy(*buffer,formatedstring);
    free(formatedstring);

}

void getVarInt(char **buffer,char *val)
{
    int value = atoi(val);
    unsigned size = snprintf(NULL, 0, "int@%d", value);
    *buffer = (char *)malloc(size + 1);
    snprintf(*buffer, size+1, "int@%d", value);
}

void getVarFloat(char **buffer,char *val)
{
    double value = atof(val);
    unsigned size = snprintf(NULL, 0, "float@%a", value);
    *buffer = (char *)malloc(size + 1);
    snprintf(*buffer, size+1, "float@%a", value);
}

void getVarBool(char **buffer,bool value)
{
    unsigned size = strlen("bool@TRUE");
    if(value){
        *buffer = (char *)malloc(size + 1);
        strcpy(*buffer,"bool@TRUE");
    }
    else{
        *buffer = (char *)malloc(size + 2);
        strcpy(*buffer,"bool@FALSE");
    }
}

void getVarString(char **buffer,char *value)
{
    unsigned size = strlen(value);
    if(size == 0)
    {
        *buffer = malloc(sizeof(char)*8);
        strcpy(*buffer,"string@");
    }
    else
    {
        char *string = NULL;
        string = malloc(size + 2);
        strcpy(string,value);
        convertToString(&string);

        size = snprintf(NULL, 0, "string@%s", string);
        *buffer = (char *)malloc(size + 1);
        snprintf(*buffer, size+1, "string@%s", string);
        free(string);
    }

}

/* GENEROVACIE FUNKCIE */

void printHead()
{
    fprintf(stdout, ".IFJcode18\nJUMP $MAIN\n");
}

void printAssign(tDLList *L,char *into, char *from)
{
    char *buffer = NULL;
    fillString(&buffer, "MOVE  LF@%s LF@%s\n",into,from);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printAssignExpr(tDLList *L,char *name)
{
    char *buffer = NULL;
    fillString(&buffer, "POPS LF@%s\n",name);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printAssignFunc(tDLList *L,char *name)
{
    char *buffer = NULL;
    fillString(&buffer, "MOVE LF@%s TF@$retval\n",name);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printDefine(tDLList *L,char *name)
{
    char *buffer = NULL;
    fillString(&buffer, "DEFVAR LF@%s\n",name);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printRetval(tDLList *L,char *name)
{
    char *buffer = NULL;
    fillString(&buffer, "MOVE LF@$retval LF@%s\n",name);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printFuncReturn(tDLList *L)
{
    char *buffer = NULL;
    fillString(&buffer, "MOVE LF@$retval TF@$retval\n");
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printParam(tDLList *L,TYPES type,char *value)
{
    char *buffer = NULL;
    char *var = NULL;
    switch (type) {
        case NONE:
            fillString(&buffer,"DEFVAR TF@$param%d\n",id_Param);
            DLPreInsert(L,buffer);
            free(buffer);
            buffer = NULL;
            fillString(&buffer,"MOVE TF@$param%d LF@%s\n ", id_Param, value);
            break;
        case INT:
            fillString(&buffer,"DEFVAR TF@$param%d\n",id_Param);
            DLPreInsert(L,buffer);
            free(buffer);
            buffer = NULL;
            fillString(&buffer,"MOVE TF@$param%d ",id_Param);
            getVarInt(&var,value);
            addString(&buffer,var);
            addString(&buffer,"\n");
            break;
        case FLOAT:
            fillString(&buffer,"DEFVAR TF@$param%d\n",id_Param);
            DLPreInsert(L,buffer);
            free(buffer);
            buffer = NULL;
            fillString(&buffer,"MOVE TF@$param%d ",id_Param);
            getVarFloat(&var,value);
            addString(&buffer,var);
            addString(&buffer,"\n");
            break;
        case STRING:
            fillString(&buffer,"DEFVAR TF@$param%d\n",id_Param);
            DLPreInsert(L,buffer);
            free(buffer);
            buffer = NULL;
            fillString(&buffer,"MOVE TF@$param%d ",id_Param);
            getVarString(&var,value);
            addString(&buffer,var);
            addString(&buffer,"\n");
            break;
    }
    id_Param ++;
    DLPreInsert(L,buffer);
    free(buffer);
    free(var);
}

void printDefParam(tDLList *L, char *name){
    char *buffer = NULL;
    fillString(&buffer, "DEFVAR LF@%s\n"
                        "MOVE LF@%s LF@$param%d\n"
                        ,name, name, id_defParam);

    DLPostInsert(L, buffer);
    DLSucc(L);
    id_defParam++;
    free(buffer);
}

void lastParam(){
    id_defParam = 1;
}

void printCallFunc(tDLList *L,char *name)
{
    char *buffer = NULL;
    fillString(&buffer,"CALL %s\n",name);
    DLPostInsert(L,buffer);
    free(buffer);
    id_Param = 1;
    DLSucc(L);
}

void printFuncLength(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer,"\nLABEL length\n"
                      "PUSHFRAME\n"
                      "DEFVAR LF@$retval\n"
                      "STRLEN LF@$retval LF@$param1\n"
                      "POPFRAME\n"
                      "RETURN\n");
    DLInsertFirst(L,buffer);
    free(buffer);
}

void printFuncSubstr(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer,  "\nLABEL substr\n"
                        "PUSHFRAME\n"
                        "DEFVAR LF@$retval\n"
                        "MOVE LF@$retval string@\n"
                        "DEFVAR LF@length_str\n"
                        "CREATEFRAME\n"
                        "DEFVAR TF@$param1\n"
                        "MOVE TF@$param1 LF@$param1\n"
                        "CALL length\n"
                        "MOVE LF@length_str TF@$retval\n"
                        "DEFVAR LF@ret_cond\n"
                        "LT LF@ret_cond LF@length_str int@0\n"
                        "JUMPIFEQ $SUBSTR$RETURN LF@ret_cond bool@true\n"
                        "EQ LF@ret_cond LF@length_str int@0\n"
                        "JUMPIFEQ $SUBSTR$RETURN LF@ret_cond bool@true\n"
                        "LT LF@ret_cond LF@$param2 int@0\n"
                        "JUMPIFEQ $SUBSTR$RETURN LF@ret_cond bool@true\n"
                        "GT LF@ret_cond LF@$param2 LF@length_str\n"
                        "JUMPIFEQ $SUBSTR$RETURN LF@ret_cond bool@true\n"
                        "EQ LF@ret_cond LF@$param3 int@0\n"
                        "JUMPIFEQ $SUBSTR$RETURN LF@ret_cond bool@true\n"
                        "DEFVAR LF@max_n\n"
                        "MOVE LF@max_n LF@length_str\n"
                        "SUB LF@max_n LF@max_n LF@$param2\n"
                        "DEFVAR LF@edit_n_cond\n"
                        "LT LF@edit_n_cond LF@$param3 int@0\n"
                        "JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n"
                        "GT LF@edit_n_cond LF@$param3 LF@max_n\n"
                        "JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n"
                        "JUMP $substr$process\n"
                        "LABEL $substr$edit_n\n"
                        "MOVE LF@$param3 LF@max_n\n"
                        "LABEL $substr$process\n"
                        "DEFVAR LF@index\n"
                        "MOVE LF@index LF@$param2\n"
                        "DEFVAR LF@char\n"
                        "DEFVAR LF@vysl\n"
                        "LABEL $SUBSTR$LOOP\n"
                        "GETCHAR LF@char LF@$param1 LF@index\n"
                        "CONCAT LF@$retval LF@$retval LF@char\n"
                        "ADD LF@index LF@index int@1\n"
                        "SUB LF@$param3 LF@$param3 int@1\n"
                        "GT LF@vysl LF@$param3 int@0\n"
                        "JUMPIFEQ $SUBSTR$LOOP LF@vysl bool@true\n"
                        "LABEL $SUBSTR$RETURN\n"
                        "DEFVAR LF@final\n"
                        "EQ LF@final LF@$retval string@\n"
                        "JUMPIFEQ $NOTNIL LF@final bool@FALSE\n"
                        "MOVE LF@$retval nil@nil\n"
                        "LABEL $NOTNIL\n"
                        "POPFRAME\n"
                        "RETURN\n" );
    DLInsertFirst(L,buffer);
    free(buffer);
}


void printFuncChr(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer,  "\nLABEL chr\n"
                        "PUSHFRAME\n"
                        "DEFVAR LF@$retval\n"
                        "DEFVAR LF@vysl\n"
                        "LT LF@vysl LF@$param1 int@0\n"
                        "JUMPIFEQ $ENDIF$CHR LF@vysl bool@TRUE\n"
                        "LT LF@vysl LF@$param1 int@256\n"
                        "JUMPIFEQ $ENDIF$CHR LF@vysl bool@FALSE\n"
                        "INT2CHAR LF@$retval LF@$param1\n"
                        "JUMP $ISOK$CHR\n"
                        "LABEL $ENDIF$CHR\n"
                        "EXIT int@4\n"
                        "LABEL $ISOK$CHR"
                        "POPFRAME\n"
                        "RETURN\n");
    DLInsertFirst(L,buffer);
    free(buffer);
}

void printFuncOrd(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer,  "\nLABEL ord\n"
                        "PUSHFRAME\n"
                        "DEFVAR LF@$retval\n"
                        "MOVE LF@$retval nil@nil\n"
                        "DEFVAR LF@vysl\n"
                        "DEFVAR LF@length\n"
                        "STRLEN LF@length LF@$param1\n"
                        "LT LF@vysl LF@$param2 int@0\n"
                        "JUMPIFEQ $ENDIF$ORD LF@vysl bool@TRUE\n"
                        "LT LF@vysl LF@$param2 LF@length\n"
                        "JUMPIFEQ $ENDIF$ORD LF@vysl bool@FALSE\n"
                        "STRI2INT LF@$retval LF@$param1 LF@$param2\n"
                        "LABEL $ENDIF$ORD\n"
                        "POPFRAME\n"
                        "RETURN\n");
    DLInsertFirst(L,buffer);
    free(buffer);
}

void printAllFunc(tDLList *L){
    printFuncLength(L);
    printFuncSubstr(L);
    printFuncChr(L);
    printFuncOrd(L);
}

void printFuncBegin(tDLList *L, char *fID)
{
    char *buffer = NULL;
    fillString(&buffer, "\nLABEL %s\n"
                        "PUSHFRAME\n"
                        "DEFVAR LF@$retval\n"
                        ,fID);
    DLInsertFirst(L, buffer);
    DLFirst(L);
    free(buffer);
}

void printFuncEnd(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer, "POPFRAME\n"
                        "RETURN\n\n");
    DLPostInsert(L, buffer);
    DLLast(L);
    free(buffer);
}

void printDumpInput(tDLList *L, TYPES type)
{
    char *buffer = NULL;
    switch (type){
        case INT:
            fillString(&buffer,"DEFVAR LF@$dump%d\n",id_dump);
            DLPostInsert(L,buffer);
            DLSucc(L);
            free(buffer);
            buffer = NULL;
            fillString(&buffer,"READ LF@$dump%d int\n",id_dump);
            break;
        case FLOAT:
            fillString(&buffer,"DEFVAR LF@$dump%d\n",id_dump);
            DLPostInsert(L,buffer);
            DLSucc(L);
            free(buffer);
            buffer = NULL;
            fillString(&buffer,"READ LF@$dump%d float\n",id_dump);
            break;
        case STRING:
            fillString(&buffer,"DEFVAR LF@$dump%d\n",id_dump);
            DLPostInsert(L,buffer);
            DLSucc(L);
            free(buffer);
            buffer = NULL;
            fillString(&buffer,"READ LF@$dump%d string\n",id_dump);
            break;
    }
    id_dump++;
    DLPostInsert(L,buffer);
    DLSucc(L);
    free(buffer);
}

void printFuncInput(tDLList *L, TYPES type, char *name)
{
    char *buffer = NULL;
    switch (type){
        case INT:
            fillString(&buffer, "READ LF@%s int\n", name);
            break;
        case FLOAT:
            fillString(&buffer,"READ LF@%s float\n", name);
            break;
        case STRING:
            fillString(&buffer,"READ LF@%s string\n", name);
            break;
    }
    id_dump++;
    DLPostInsert(L,buffer);
    DLSucc(L);
    free(buffer);
}

void printWrite( tDLList *L, TYPES type, char *name)
{
    char *buffer = NULL;
    char *var = NULL;
    switch (type) {
        case NONE:
            fillString(&buffer, "WRITE LF@%s\n", name);
            break;
        case INT:
            fillString(&buffer, "WRITE ");
            getVarInt(&var,name);
            addString(&buffer,var);
            addString(&buffer,"\n");
            break;
        case FLOAT:
            fillString(&buffer, "WRITE ");
            getVarFloat(&var,name);
            addString(&buffer,var);
            addString(&buffer,"\n");
            break;
        case STRING:
            fillString(&buffer, "WRITE ");
            getVarString(&var,name);
            addString(&buffer,var);
            addString(&buffer,"\n");
            break;
    }
    DLPostInsert(L,buffer);
    DLSucc(L);
    free(buffer);
    free(var);
}

void printMainBegin(tDLList *L)
{
    char *buffer = NULL;
    addString(&buffer, "LABEL $MAIN\nCREATEFRAME\nPUSHFRAME\n");
    DLInsertFirst(L, buffer);
    DLFirst(L);
    free(buffer);
}

void printMainEnd()
{
    fprintf(stdout, "CLEARS\nPOPFRAME\n");
}

void printIf(tDLList *L){
    char *buffer = NULL;
    id_if++;
    SPush(&stack,id_if);
    fillString(&buffer, "DEFVAR LF@if$cond%d\n"
                        "DEFVAR LF@$type%d\n",id_if,id_if);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
    buffer = NULL;
    fillString(&buffer, "POPS LF@if$cond%d\n"
                        "TYPE LF@$type%d LF@if$cond%d\n"
                        "JUMPIFEQ $else%d LF@$type%d string@nil\n"
                        "JUMPIFEQ $isint%d LF@$type%d string@int\n"
                        "JUMPIFEQ $isfloat%d LF@$type%d string@float\n"
                        "JUMPIFEQ $isbool%d LF@$type%d string@bool\n"
                        "LABEL $isint%d\n"
                        "JUMPIFEQ $else%d LF@if$cond%d int@0\n"
                        "JUMP $if%d\n"
                        "LABEL $isfloat%d\n"
                        "JUMPIFEQ $else%d LF@if$cond%d float@0x0p+0\n"
                        "JUMP $if%d\n"
                        "LABEL $isbool%d\n"
                        "JUMPIFEQ $else%d LF@if$cond%d bool@false\n"
                        "LABEL $if%d\n"
                        ,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if, id_if, id_if
                     ,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if);

    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printElse(tDLList *L){
    char *buffer = NULL;
    int id_print = STop(&stack);
    fillString(&buffer,  "JUMP $endif%d\n"
                        "LABEL $else%d\n"
                        , id_print, id_print);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printEndif(tDLList *L){
    char *buffer = NULL;
    int id_print = STopPop(&stack);
    fillString(&buffer,  "LABEL $endif%d\n"
                        ,id_print);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printWhile(tDLList *L){
    char *buffer = NULL;
    id_if++;
    SPush(&stack,id_if);
    fillString(&buffer,  "LABEL start$while%d\n", id_if);

    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
    buffer = NULL;

    fillString(&buffer, "DEFVAR LF@while$cond%d\n"
                        "DEFVAR LF@$type%d\n",id_if,id_if);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
    buffer = NULL;
    fillString(&buffer, "POPS LF@while$cond%d\n"
                        "TYPE LF@$type%d LF@while$cond%d\n"
                        "JUMPIFEQ $endwhile%d LF@$type%d string@nil\n"
                        "JUMPIFEQ $isint%d LF@$type%d string@int\n"
                        "JUMPIFEQ $isfloat%d LF@$type%d string@float\n"
                        "JUMPIFEQ $isbool%d LF@$type%d string@bool\n"
                        "LABEL $isint%d\n"
                        "JUMPIFEQ $endwhile%d LF@while$cond%d int@0\n"
                        "JUMP $while%d\n"
                        "LABEL $isfloat%d\n"
                        "JUMPIFEQ $endwhile%d LF@while$cond%d float@0x0p+0\n"
                        "JUMP $while%d\n"
                        "LABEL $isbool%d\n"
                        "JUMPIFEQ $endwhile%d LF@while$cond%d bool@false\n"
                        "LABEL $while%d\t\t\t\t#koniecPRINTWHILE\n"
                        ,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if
                ,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if,id_if);

    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);
}

void printEndwhile(tDLList *L){
    char *buffer = NULL;
    int id_print = STopPop(&stack);
    fillString(&buffer, "JUMP start$while%d\n", id_print);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);

    fillString(&buffer, "LABEL $endwhile%d\n", id_print);
    DLPostInsert(L, buffer);
    DLSucc(L);
    free(buffer);

}
