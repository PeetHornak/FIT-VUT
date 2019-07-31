#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "header/parser.h"

int param_count = 0;
int def_count = 0;
int print_count = 0;
int if_count = 0;
int while_count = 0;
int one_more_global_id = 0;
static tDLList L;

bool parse()
{
    char *buffer = NULL;
    DLInitList(&L);
    printMainBegin(&L);
    tokenType token = getNextToken(&buffer);
    bool ret = st_list(&token, &buffer);
    check_defined();
    printHead();
    printAllFunc(&L);
    checkIf();
    checkWhile();
    DLPrintList(&L);
    printMainEnd();
    return ret;
}



bool st_list(tokenType* token, char** buffer)
{
    //////////////////////////////////////////////////// 01
    if(*token != TYPE_EOF)
        return (stat(token, buffer) && st_list(token, buffer));  //<STAT> <ST-LIST>
    else
    //////////////////////////////////////////////////// 02
        return true;                        //EOF
}

bool stat(tokenType* token, char** buffer)
{
    //////////////////////////////////////////////////// 03
    if(*token == TYPE_ID){                   //ID
        if(id_item(token, buffer))        //<ID-ITEM>
            return true;
        else
            gb_exit_process(2);
    }

    else if((*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO) || *token == TYPE_L_BRE || *token == TYPE_NIL){   //<EXPR>
        ungetToken(*token, *buffer);
        TGLOBTab *htab_temp = htab_return_pointer();
        if(expression_parse(TYPE_EOL, NULL, htab_temp->loc_symtab, &L)){
            if(def_count != 0)
                DLPostInsert(&L, "POPS LF@retval\n");
            *token = getNextToken(buffer);
            return true;
        }
        gb_exit_process(2);
    }

    //////////////////////////////////////////////////// 04
    else if(*token == TYPE_FUNC_ID || *token == TYPE_PRE_FUNC){              //FUNC_ID
        TGLOBTab* my_glob_obj = htab_call_func(*buffer);        // Vloz do htab FUNC_ID undefined
        if(**buffer == 'i'){            // predef func jedna s input
            one_more_global_id++;
            switch ((*buffer)[5]) {
                case 's':
                    printDumpInput(&L, STRING);
                    break;
                case 'i' :
                    printDumpInput(&L, INT);
                    break;
                case 'f' :
                    printDumpInput(&L, FLOAT);
                    break;
            }
        }
        else if(**buffer == 'p'){        // predef func print
            one_more_global_id++;
        }
        else{
            printCallFunc(&L, *buffer);                     // Volanie funkcie, params preinsert
            DLPreInsert(&L, "CREATEFRAME\n");                // CREATEFRAME pred parametrami
        }
        *token = getNextToken(buffer);
        if(func(token, buffer)){          //<FUNC>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            if(one_more_global_id != 0)        //Ak je to input or read nepriraduj
                if(def_count != 0){                         // Vo funkcii prirad to retval nil
                    DLPostInsert(&L, "MOVE LF@$retval nil@nil\n");
                    DLSucc(&L);
                }
                one_more_global_id--;
                return true;
            if(def_count != 0){
                printFuncReturn(&L);
            }
            return true;
        }
        else
            gb_exit_process(2);
    }

    ///////////////////////////////////////////////////// 05
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "if")){     // IF
        if_count++;
        printIf(&L);
        TGLOBTab *htab_temp = htab_return_pointer();
        if(expression_parse(TYPE_KEYWORD, "then", htab_temp->loc_symtab, &L)){       //<EXPRESSION>
            *token = getNextToken(buffer);
            if(*token == TYPE_EOL){      //EOL
                *token = getNextToken(buffer);
                if(else_st_list(token, buffer))     //<ELSE-ST-LIST>
                    if(*token == TYPE_EOL){      //EOL
                        printElse(&L);
                        *token = getNextToken(buffer);
                        if(end_st_list(token, buffer))    //<END-ST-LIST>
                            if(*token == TYPE_EOL){      //EOL
                                printEndif(&L);
                                *token = getNextToken(buffer);
                                //endif
                                if_count--;
                                return true;
                            }
                    }
            }
        }
        gb_exit_process(2);
    }
    ////////////////////////////////////////////////////// 06
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "while")){      //WHILE 
        while_count++;                          //TODO: DEFVAR PRED WHILE
        printWhile(&L);
        TGLOBTab *htab_temp = htab_return_pointer();
        if(expression_parse(TYPE_KEYWORD, "do", htab_temp->loc_symtab, &L)){       //<EXPRESSION>
            *token = getNextToken(buffer);
            if(*token == TYPE_EOL){      //EOL
                *token = getNextToken(buffer);
                if(end_st_list(token, buffer))     //<END-ST-LIST>
                    if(*token == TYPE_EOL){      //EOL
                        printEndwhile(&L);
                        *token = getNextToken(buffer);
                        while_count--;
                        return true;
                    }
            }
        }
        gb_exit_process(2);
    }
    ////////////////////////////////////////////////////// 07
    else if(*token == TYPE_KEYWORD && !strcmp(*buffer, "def")){      //DEF
        if(def_count > 0 || if_count > 0 || while_count > 0){
            printf("ERROR: definition inside if,while,def\n");
            gb_exit_process(2);
        }
        def_count++;
        *token = getNextToken(buffer);
        if(*token == TYPE_ID || *token == TYPE_FUNC_ID){       //ID
            TGLOBTab* my_glob_obj = htab_def_func(*buffer);
            printFuncBegin(&L, *buffer);
            *token = getNextToken(buffer);
            if(*token == TYPE_L_BRE){
                *token = getNextToken(buffer);
                if(bracket(token, buffer)){
                    print_count++;
                    htab_set_param_count(my_glob_obj, param_count);
                    param_count = 0;
                    lastParam();
                    if(end_st_list(token, buffer)){   // <FUNC> && <END_ST_LIST>
                        if(*token == TYPE_EOL){   //EOL
                            printFuncEnd(&L);
                            def_count--;
                            print_count = 0;
                            htab_set_main();
                            return true;
                        }
                    }
                }
            }
        }
        gb_exit_process(2);
    }
    else if(*token == TYPE_EOL){
        *token = getNextToken(buffer);
        return true;
    }
    else
        gb_exit_process(2);
}

bool else_st_list(tokenType* token, char** buffer)
{
    //////////////////////////////////////////////////////// 08
    if(*token == TYPE_KEYWORD && !strcmp(*buffer, "else")){   //ELSE
        *token = getNextToken(buffer);
        return true;
    }   
    //////////////////////////////////////////////////////// 09
    else{
        if(stat(token, buffer) && else_st_list(token, buffer))   //<STAT> && <ELSE_ST_LIST>
            return true;
        else
            gb_exit_process(2);
    }
}

bool end_st_list(tokenType* token, char** buffer)
{
    //////////////////////////////////////////////////////// 10
    if(*token == TYPE_KEYWORD && !strcmp(*buffer, "end")){     //END
        *token = getNextToken(buffer);
        return true;
    }       
    //////////////////////////////////////////////////////// 11
    else{ 
        if(stat(token, buffer) && end_st_list(token, buffer))   //<STAT> && <END_ST_LIST>
            return true;
        else
            gb_exit_process(2);
    }
}

bool id_item(tokenType* token, char** buffer)
{
    tokenType temp = *token;
    char* temp_buff = *buffer;
    *token = getNextToken(buffer);
    //////////////////////////////////////// 12
    if(*token == TYPE_ASSIGN){                   // =
        if(check_id(temp_buff) == false)
            printDefine(&L, temp_buff);     //inak aj definuj
        htab_add_id(temp_buff);
        *token = getNextToken(buffer);
        if(assign(token, buffer, temp_buff))            //<ASSIGN>
            return true;
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 13
    else if(*token == TYPE_EOL){                 //EOL
        htab_find_id(temp_buff);
        if(check_id(temp_buff)){            //ak je to id
            if(def_count != 0){             //a sme vo func
                printRetval(&L, temp_buff);
            }
            else{}
        }
        else{                               //tak to je func
            printCallFunc(&L, temp_buff);                 // Volanie funkcie bez parametrov
            DLPreInsert(&L, "CREATEFRAME\n");           // CREATEFRAME pred parametrami
            if(def_count != 0)
                printFuncReturn(&L);
        }    
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 14
    else if(*token >= TYPE_PLUS && *token <= TYPE_NEG_EQUAL){     //SIGN
        ungetToken(temp, temp_buff);
        ungetToken(*token, *buffer);
        TGLOBTab *htab_temp = htab_return_pointer();
        if(expression_parse(TYPE_EOL, NULL, htab_temp->loc_symtab, &L)){
            if(def_count != 0)
                DLPostInsert(&L, "POPS LF@retval\n");
            *token = getNextToken(buffer);
            return true;
        }
            
    }
    else{
        TGLOBTab* my_glob_obj = htab_call_func(temp_buff);        // Vloz do htab FUNC_ID undefined
        printCallFunc(&L, temp_buff);                         // Volanie funkcie, params preinsert
        DLPreInsert(&L, "CREATEFRAME\n");                      // CREATEFRAME pred parametrami
        if(func(token, buffer)){              //<FUNC>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            if(def_count != 0){             //sme vo func
                printFuncReturn(&L);
            }
            return true;
        }
        else
            gb_exit_process(2);
    }
}

bool assign(tokenType* token, char** buffer, char* id_buffer)
{
    //////////////////////////////////////// 15
    if(*token == TYPE_ID){                       //ID
        if(next(token, buffer, id_buffer))            //<NEXT>
            return true;
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 16
    else if(*token == TYPE_FUNC_ID || *token == TYPE_PRE_FUNC){             //FUNC_ID
        TGLOBTab* my_glob_obj = htab_call_func(*buffer);
        if(**buffer == 'i'){            // predef func jedna s input
            one_more_global_id++;
            switch ((*buffer)[5]) {
                case 's':
                    printFuncInput(&L, STRING, id_buffer);
                    break;
                case 'i' :
                    printFuncInput(&L, INT, id_buffer);
                    break;
                case 'f' :
                    printFuncInput(&L, FLOAT, id_buffer);
                    break;
            }
        }
        else if(**buffer == 'p'){        // predef func print
            one_more_global_id++;
        }
        else{
            printCallFunc(&L, *buffer);                     // Volanie funkcie, params preinsert
            DLPreInsert(&L, "CREATEFRAME\n");                // CREATEFRAME pred parametrami
        }
        *token = getNextToken(buffer);
        if(func(token, buffer)){              //<FUNC>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            if(one_more_global_id != 0){        //Ak je to input or read nepriraduj
                if(def_count != 0){                         // Vo funkcii prirad to retval nil
                    DLPostInsert(&L, "MOVE LF@$retval nil@nil\n");
                    DLSucc(&L);
                }
                one_more_global_id--;
                return true;
            }
            printAssignFunc(&L, id_buffer);             //prirad funkciu
            if(def_count != 0){             //a sme vo func
                printRetval(&L, id_buffer);   
            }
            return true;
        }
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 17
    else if((*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO) || *token == TYPE_L_BRE || *token == TYPE_NIL){   //<EXPR>
        ungetToken(*token, *buffer);
        TGLOBTab *htab_temp = htab_return_pointer();
        printAssignExpr(&L, id_buffer); //prirad
        if(expression_parse(TYPE_EOL, NULL, htab_temp->loc_symtab, &L)){
            if(def_count != 0)                 //ak sme vo func tak prirad to retval
                printRetval(&L, id_buffer);
            *token = getNextToken(buffer);
            return true;
        }
        else
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
}

bool next(tokenType* token, char** buffer, char *id_buffer)
{
    tokenType temp = *token;
    char* temp_buff = *buffer;
    *token = getNextToken(buffer);
    //////////////////////////////////////// 18
    if(*token == TYPE_EOL){                      //EOL
        htab_find_id(temp_buff);
        if(check_id(temp_buff)){            //ak je to id
            printAssign(&L, id_buffer, temp_buff);
            if(def_count != 0){             //a sme vo func
                printRetval(&L, temp_buff);
            }
            else{}
        }
        else{                               //tak to je func
            printCallFunc(&L, temp_buff);                 // Volanie funkcie, bez params
            DLPreInsert(&L, "CREATEFRAME\n");           // CREATEFRAME pred parametrami
            printAssignFunc(&L, id_buffer);
            if(def_count != 0){             //a sme vo func
                printFuncReturn(&L);
            }
            else{}
        }    
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 19
    else if(*token >= TYPE_PLUS && *token <= TYPE_NEG_EQUAL){     //SIGN
        ungetToken(temp, temp_buff);
        ungetToken(*token, *buffer);
        TGLOBTab *htab_temp = htab_return_pointer();
        printAssignExpr(&L, id_buffer); //prirad
        if(expression_parse(TYPE_EOL, NULL, htab_temp->loc_symtab, &L)){                  //<EXPR>
            if(def_count != 0){                 //ak sme vo func tak prirad to retval
                printRetval(&L, id_buffer);
            }
            *token = getNextToken(buffer);
            return true;
        }
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 20
    else if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO) || *token == TYPE_NIL){  //ID|VALUE
        TGLOBTab* my_glob_obj = htab_call_func(temp_buff);
        printCallFunc(&L, temp_buff);                 // Volanie funkcie, params preinsert
        DLPreInsert(&L, "CREATEFRAME\n");           // CREATEFRAME pred parametrami
        if(param(token, buffer)){             //<PARAM>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            printAssignFunc(&L, id_buffer);
            if(def_count != 0){
                printRetval(&L, id_buffer);
            }
            return true;
        }
        else
            gb_exit_process(2);
    }
    //////////////////////////////////////// 21
    else if(*token == TYPE_L_BRE){               // (
        TGLOBTab* my_glob_obj = htab_call_func(temp_buff);
        printCallFunc(&L, temp_buff);                 // Volanie funkcie, params preinsert
        DLPreInsert(&L, "CREATEFRAME\n");           // CREATEFRAME pred parametrami
        *token = getNextToken(buffer);
        if(bracket(token, buffer)){         //<BRACKET>
            htab_set_param_count(my_glob_obj, param_count);
            param_count = 0;
            printAssignFunc(&L, id_buffer);
            if(def_count != 0){
                printRetval(&L, id_buffer);
            }
            if(*token == TYPE_EOL){             //EOL
                *token = getNextToken(buffer);
                return true;
            }
        }
        gb_exit_process(2);
    }
    else
        gb_exit_process(2);
}

bool func(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 22
    if(*token == TYPE_L_BRE){               // (
        *token = getNextToken(buffer);
        if(bracket(token, buffer))         //<BRACKET
            if(*token == TYPE_EOL){             //EOL
                *token = getNextToken(buffer);
                return true;
            }
        gb_exit_process(2);
    //////////////////////////////////////// 23
    }
    else if(*token == TYPE_EOL){            // EOL
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 24
    else{
        if(param(token, buffer))             //<PARAM>
            return true;
        else
            gb_exit_process(2);
    }
}

bool param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 25
    if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO || *token == TYPE_NIL)){   // PARAMETER
        if(*token == TYPE_ID){
            if(one_more_global_id != 0){
                printWrite(&L, NONE, *buffer);
            }
            else{
                printParam(&L, NONE, *buffer);
                htab_check_param(*buffer);
            }
        }
        else if(*token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){
            if(one_more_global_id != 0)
                printWrite(&L, STRING, *buffer);
            else
                printParam(&L, STRING, *buffer);
        }
        else if(*token == TYPE_NIL){
            printParam(&L, NIL, *buffer);
        }
        else if(*token == TYPE_INT || *token == TYPE_INT_EXPO){
            if(one_more_global_id != 0)
                printWrite(&L, INT, *buffer);
            else
                printParam(&L, INT, *buffer);
        }
        else if(*token = TYPE_FLOAT || *token == TYPE_FLOAT_EXPO){
            if(one_more_global_id != 0)
                printWrite(&L, FLOAT, *buffer);
            else
            printParam(&L, FLOAT, *buffer);
        }
        *token = getNextToken(buffer);
        param_count++;
        if(next_param(token, buffer))        //<NEXT-PARAM>
            return true;
        else
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
}

bool next_param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 26
    if(*token == TYPE_EOL){                      //EOL
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 27
    else if(*token == TYPE_COMMA){               // ,
        *token = getNextToken(buffer);
        if(param(token, buffer))             //<PARAM>
            return true;
        else
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
}

bool bracket(tokenType* token, char** buffer){
    //////////////////////////////////////// 28
    if(*token == TYPE_R_BRE){                    // )
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 29
    else
        if(brc_param(token, buffer))         //<BRC-PARAM>
            return true;
        else
            gb_exit_process(2);
}

bool brc_param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 30
    if(*token == TYPE_ID || (*token >= TYPE_QUOT && *token <= TYPE_FLOAT_EXPO) || *token == TYPE_NIL){   // PARAMETER
        if(def_count == 0){          // Ak volam funckiu 
            if(*token == TYPE_ID){
                if(one_more_global_id != 0){
                    printWrite(&L, NONE, *buffer);
                    htab_check_param(*buffer);
                }
                else{
                    printParam(&L, NONE, *buffer);
                    htab_check_param(*buffer);
                }
            }
            else if(*token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){
                if(one_more_global_id != 0)
                    printWrite(&L, STRING, *buffer);
                else
                    printParam(&L, STRING, *buffer);
            }
            else if(*token == TYPE_NIL){
                printParam(&L, NIL, *buffer);
            }
            else if(*token == TYPE_INT || *token == TYPE_INT_EXPO){
                if(one_more_global_id != 0)
                    printWrite(&L, INT, *buffer);
                else
                    printParam(&L, INT, *buffer);
            }
            else if(*token = TYPE_FLOAT || *token == TYPE_FLOAT_EXPO){
                if(one_more_global_id != 0)
                    printWrite(&L, FLOAT, *buffer);
                else
                    printParam(&L, FLOAT, *buffer);
            }
        }
        else{
            if(print_count == 0){
                printDefParam(&L, *buffer);
                if(*token == TYPE_ID){
                    if(def_count != 0){
                        htab_def_param(*buffer);
                    }
                }
            }
            else{
                if(*token == TYPE_ID){
                    if(one_more_global_id != 0)
                        printWrite(&L, NONE, *buffer);
                    else{
                        printParam(&L, NONE, *buffer);
                        htab_check_param(*buffer);
                    }
                }
                else if(*token == TYPE_QUOT || *token == TYPE_QUOT_EMPTY){
                    if(one_more_global_id != 0)
                        printWrite(&L, STRING, *buffer);
                    else
                        printParam(&L, STRING, *buffer);
                }
                else if(*token == TYPE_NIL){
                    printParam(&L, NIL, *buffer);
                }
                else if(*token == TYPE_INT || *token == TYPE_INT_EXPO){
                    if(one_more_global_id != 0)
                        printWrite(&L, INT, *buffer);
                    else
                        printParam(&L, INT, *buffer);
                }
                else if(*token = TYPE_FLOAT || *token == TYPE_FLOAT_EXPO){
                    if(one_more_global_id != 0)
                        printWrite(&L, FLOAT, *buffer);
                    else
                        printParam(&L, FLOAT, *buffer);
                }
            }
        }

        param_count++;
        *token = getNextToken(buffer);
        if(next_brc_param(token, buffer))    //<NEXT-BRC-PARAM>
            return true;
        else
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
}

bool next_brc_param(tokenType* token, char** buffer)
{
    //////////////////////////////////////// 31
    if(*token == TYPE_R_BRE){                    // )
        *token = getNextToken(buffer);
        return true;
    }
    //////////////////////////////////////// 32
    else if(*token == TYPE_COMMA){              // ,
        *token = getNextToken(buffer);
        if(brc_param(token, buffer))         //<BRC-PARAM>
            return true;
        else
            gb_exit_process(2);
    }
    else
        gb_exit_process(2);
}

void checkIf()
{
    int check = 0;
    tDLElemPtr temp_ptr, my_ptr, iterate = L.First;
    while(iterate != NULL){
        my_ptr = iterate;
        iterate = iterate->rptr;
        if(strlen(my_ptr->instruction) >= 15)
            if(strncmp(my_ptr->instruction, "POPS LF@if$cond", 15) == 0){
                if(check == 0){
                    temp_ptr = my_ptr;
                }
                check++;
            }

        if(strlen(my_ptr->instruction) >= 12)
            if(strncmp(my_ptr->instruction, "LABEL $endif", 12) == 0){
                check--;
            }

        if(check){
            if(strlen(my_ptr->instruction) >= 9)
                if(strncmp(my_ptr->instruction, "DEFVAR LF", 9) == 0){
                    my_ptr->lptr->rptr = my_ptr->rptr;
                    my_ptr->rptr->lptr = my_ptr->lptr;
                    my_ptr->rptr = temp_ptr;
                    my_ptr->lptr = temp_ptr->lptr;
                    my_ptr->lptr->rptr = my_ptr;
                    my_ptr->rptr->lptr = my_ptr;
                }
        }
        
    }
}

void checkWhile()
{
    int check = 0;
    tDLElemPtr temp_ptr, my_ptr, iterate = L.First;
    while(iterate != NULL){
        my_ptr = iterate;
        iterate = iterate->rptr;
        if(strlen(my_ptr->instruction) >= 17)
            if(strncmp(my_ptr->instruction, "LABEL start$while", 17) == 0){
                if(check == 0){
                    temp_ptr = my_ptr;
                }
                check++;
            }

        if(strlen(my_ptr->instruction) >= 15)
            if(strncmp(my_ptr->instruction, "LABEL $endwhile", 15) == 0){
                check--;
            }

        if(check){
            if(strlen(my_ptr->instruction) >= 9)
                if(strncmp(my_ptr->instruction, "DEFVAR LF", 9) == 0){
                    my_ptr->lptr->rptr = my_ptr->rptr;
                    my_ptr->rptr->lptr = my_ptr->lptr;
                    my_ptr->rptr = temp_ptr;
                    my_ptr->lptr = temp_ptr->lptr;
                    my_ptr->lptr->rptr = my_ptr;
                    my_ptr->rptr->lptr = my_ptr;
                }
        }
        
    }
}
