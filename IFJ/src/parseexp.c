/** parseexp.c
* This file holds functions and structures for precedence (bottom-up) analysis
* a.k.a expression parsing.
* Author: Nikolas Patrik xpatri00
        * Collaborators:
*      Peter Hornak   xhorna14
        *      Matej Jancek   xjance00
*      Robert Hubinak xhubin03
*
*/
#include <string.h>
#include <stdbool.h>

#include "header/scanner.h"
#include "header/parseexp.h"
#include "header/garbagecollector.h"
#include "header/symtable.h"
#include "header/generate.h"
#include "header/instrlist.h"


// Precedence table:
static char precedence_table[TERM_COUNT][TERM_COUNT] = {
//    + , - , * , / , ( , ) , < ,<= , > , <=, ==, !=, id, $
    {'>','>','<','<','<','>','>','>','>','>','>','>','<','>'}, //+
    {'>','>','<','<','<','>','>','>','>','>','>','>','<','>'}, //-
    {'>','>','>','>','<','>','>','>','>','>','>','>','<','>'}, //*
    {'>','>','>','>','<','>','>','>','>','>','>','>','<','>'}, ///
    {'<','<','<','<','<','=','<','<','<','<','<','<','<',' '}, //(
    {'>','>','>','>',' ','>','>','>','>','>','>','>',' ','>'}, //)
    {'<','<','<','<','<','>',' ',' ',' ',' ','>','>','<','>'}, //<
    {'<','<','<','<','<','>',' ',' ',' ',' ','>','>','<','>'}, //<=
    {'<','<','<','<','<','>',' ',' ',' ',' ','>','>','<','>'}, //>
    {'<','<','<','<','<','>',' ',' ',' ',' ','>','>','<','>'}, //>=
    {'<','<','<','<','<','>','<','<','<','<',' ',' ','<','>'}, //==
    {'<','<','<','<','<','>','<','<','<','<',' ',' ','<','>'}, //!=
    {'>','>','>','>',' ','>','>','>','>','>','>','>',' ','>'}, //id
    {'<','<','<','<','<',' ','<','<','<','<','<','<','<',' '}, //$
};

#define INFO_SIZE 256
#define BAD_MAPPING -1

void es_init(TEvalStack *t)
{
    t->stack_top = NULL;
    t->count = 0;
}

void es_push(TEvalStack *t, bool is_terminal, bool is_nonterminal,
       tokenType token, char *attribute, char info)
{
    TStackItemPtr tmp = gb_malloc(sizeof(struct stack_item));
    if( tmp == NULL)
        return;
    tmp->is_terminal = is_terminal;
    tmp->is_nonterminal = is_nonterminal;
    tmp->token = token;
    tmp->token_attribute = attribute; 
    tmp->info = info;

    tmp->prev = t->stack_top;
    t->stack_top = tmp;
    t->count++;
}

TStackItem es_top_pop(TEvalStack *t)
{
    TStackItem item = *(t->stack_top);
    TStackItemPtr tmp = t->stack_top;
    t->stack_top = tmp->prev;
    //gb_free(tmp);
    t->count--;
    return item;
}

TStackItemPtr es_top_terminal(TEvalStack *t)
{
    for(TStackItemPtr i = t->stack_top; i !=NULL; i = i->prev)
        if(i->is_terminal)
            return i;
    return NULL;
}

void es_terminal_preinsert(TEvalStack *t,bool is_terminal,
        bool is_nonterminal,tokenType token,char *attribute,
        char info)
{
    TStackItemPtr prev = NULL;
    TStackItemPtr i; 
    for(i = t->stack_top; i != NULL && !(i->is_terminal);
            i= i->prev)
        prev = i;

    if(prev == NULL)
        es_push(t,is_terminal,is_nonterminal,token,attribute,info);
    else {
        TStackItemPtr newitem = gb_malloc(sizeof(
                    struct stack_item));
        if(newitem == NULL)
            return;

        newitem->is_terminal = is_terminal;
        newitem->is_nonterminal = is_nonterminal;
        newitem->token = token;
        newitem->token_attribute = attribute;
        newitem->info = info;
        newitem->prev = i;
        prev->prev = newitem;
    }
}
                

bool es_is_empty(TEvalStack *t)
{
    return (t->count == 0) && (t->stack_top == NULL);
}

bool es_endcondition(TEvalStack *t,tokenType end, char* attribute,
        tokenType got, char* gottenAttr)
{
    /*
     * Checking if end conditon of precedence analysis is true
     */
    bool c_flag = false;
    if(end == got){
        if( attribute == NULL || gottenAttr == NULL){
            if(attribute == gottenAttr)
                c_flag = true; // We had found end token
            else
                return false;
        }
        else if(strcmp(attribute,gottenAttr) == 0)
            c_flag = true;
        else
            return false;
    }
    else
        return false;
    
    
    if(c_flag && t->stack_top->prev->info == '$' &&
            t->stack_top->info != 0)
        return true; // We had found end token and stack contains only one nonterminal and end token
    else
        return false;
}


int get_table_index(tokenType type)
{
    /**
     * This function maps all the token types into precedence table,
     * and -1 means BAD_MAPPING
     */
    int token_mapping_to_table[] = {0,1,2,3,-1,10,8,6,9,7,11,4,5,
        -1,12,12,12,12,12,12,12,-1,-1,-1,-1,-1,-1,12};
    return token_mapping_to_table[type];
}

#define END_EXP_MAPPING 13 //because 13 is index to end character in precedence table


void rule_operators(TEvalStack *t,tDLList *l,htab_t *h);
void rule_brackets(TEvalStack *t);
void rule_term_to_noterm(TEvalStack *t,tDLList *l, htab_t *h);


bool expression_parse(tokenType end, char* end_token_attribute, htab_t *symtab, 
        tDLList *l_main)
{
    TEvalStack t;
    es_init(&t);
    tokenType token;
    char *buffer = NULL;
    tDLList *l = gb_malloc(sizeof(tDLList));
    DLInitList(l);
    
    //Initialization Part
    es_push(&t, true, false, end, end_token_attribute,'$');
    TStackItemPtr a = es_top_terminal(&t);
    token = getNextToken(&buffer);
    int mapped_index1, mapped_index2;
    TStackItemPtr top1,top2;

    do {
        //Mapping Part
        if( a->token == end && a->token_attribute == end_token_attribute) 
            mapped_index1 = END_EXP_MAPPING; 
        else
            mapped_index1 = get_table_index(a->token); 
        mapped_index2 = get_table_index(token); 
        if(mapped_index2 == BAD_MAPPING){
            if(token == end) 
                mapped_index2 = END_EXP_MAPPING;
            else
                return false;
        }
        
        //Searching in Precedence Table
        switch(precedence_table[mapped_index1][mapped_index2]){
            case '=':
                es_push(&t, true, false, token, buffer, 0);
                a = es_top_terminal(&t);
                token = getNextToken(&buffer);
                break;
            case '<':
                es_terminal_preinsert(&t,false,false, -1,NULL,'<');
                es_push(&t,true,false, token, buffer, 0);
                a = es_top_terminal(&t);
                token = getNextToken(&buffer);
                break;
            case '>':
                top1= t.stack_top;
                top2= top1->prev;
                if(top2->info == '<')
                    if(top1->is_terminal)
                        rule_term_to_noterm(&t,l,symtab);
                    else
                        return false;
                else{
                    TStackItemPtr top3 = top2->prev;
                    if(top2->is_terminal){
                        if(!(top1->is_nonterminal || top3->is_nonterminal))
                            return false;

                        rule_operators(&t,l,symtab);
                    }
                    else if(top2->is_nonterminal){
                        if(!(top1->token == TYPE_R_BRE || 
                                    top3->token ==TYPE_L_BRE))
                            return false;

                        rule_brackets(&t);
                    }
                }
                a = es_top_terminal(&t);
                break;
            default:
                return false;
        }
    }while(!es_endcondition(&t,end,end_token_attribute,token,buffer));
    DLPreInsertList(l_main,l);
    return true;
}


void generate_add(tDLList *l,unsigned var_number, unsigned label_number)
{
    char *code = NULL;
    fillString(&code,
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d$type\n"
            "DEFVAR LF@prec$notype$%d$type\n",
            var_number,var_number+1,var_number+2,
            var_number,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "POPS LF@prec$notype$%d\n"
            "POPS LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n",
            var_number,var_number+1,var_number,var_number,
            var_number+1,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    /* Type control in ifjcode18 */
    fillString(&code,
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$equal$types$%d LF@prec$notype$%d$type LF@prec$notype$%d$type\n"
            "JUMPIFEQ $$equal$int$%d LF@prec$notype$%d$type string@int\n"
            "JUMPIFEQ $$equal$float$%d LF@prec$notype$%d$type string@float\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@string\n",
            label_number,var_number,label_number,var_number+1,
            label_number,var_number,label_number,var_number+1,
            label_number,var_number,var_number+1,
            label_number,var_number,label_number,var_number,
            label_number,var_number);
    DLInsertLast(l,code);
    //gb_free(code);
    /* Implicit conversions */
    fillString(&code,
            "LABEL $$equal$int$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@float\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$additions$%d\n"
            "LABEL $$equal$float$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@int\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$additions$%d\n"
            "LABEL $$equal$types$%d\n"
            "JUMPIFEQ $$string$concat$%d LF@prec$notype$%d$type string@string\n",
            label_number,label_number,var_number+1,var_number,var_number,
            label_number,label_number,label_number,var_number+1,
            var_number+1,var_number+1,label_number,label_number,
            label_number,var_number);
    DLInsertLast(l,code);
    //gb_free(code);
    /* Adding or concatenation */
    fillString(&code,
            "LABEL $$additions$%d\n"
            "ADD LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$end$exp$%d\n"
            "LABEL $$string$concat$%d\n"
            "CONCAT LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$end$exp$%d\n"
            "LABEL $$error$types$%d\n"
            "EXIT int@4\n"
            "LABEL $$end$exp$%d\n"
            "PUSHS LF@prec$notype$%d\n",
            label_number,var_number+2,var_number+1,var_number,
            label_number,label_number,var_number+2,var_number+1,
            var_number,label_number,label_number,label_number,
            var_number+2);
    DLInsertLast(l,code);
    //gb_free(code);
}

void generate_subtract(tDLList *l, unsigned var_number, unsigned label_number)
{
    char *code;
    fillString(&code,
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d$type\n"
            "DEFVAR LF@prec$notype$%d$type\n",
            var_number,var_number+1,var_number+2,
            var_number,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "POPS LF@prec$notype$%d\n"
            "POPS LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n",
            var_number,var_number+1,var_number,var_number,
            var_number+1,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@string\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@string\n"
            "JUMPIFEQ $$equal$types$%d LF@prec$notype$%d$type LF@prec$notype$%d$type\n"
            "JUMPIFEQ $$equal$int$%d LF@prec$notype$%d$type string@int\n"
            "JUMPIFEQ $$equal$float$%d LF@prec$notype$%d$type string@float\n",
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,var_number+1,
            label_number,var_number,label_number,var_number);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "LABEL $$equal$int$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@float\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n"
            "LABEL $$equal$float$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@int\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n" 
            "LABEL $$equal$types$%d\n"
            "SUB LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$end$exp$%d\n"
            "LABEL $$error$types$%d\n"
            "EXIT int@4\n"
            "LABEL $$end$exp$%d\n"
            "PUSHS LF@prec$notype$%d\n",
            label_number,label_number,var_number+1,var_number,var_number,label_number,
            label_number,label_number,var_number+1,var_number+1,var_number+1,label_number,
            label_number,var_number+2,var_number+1,var_number,label_number,
            label_number,label_number,var_number+2);
    DLInsertLast(l,code);
    //gb_free(code);
}


void generate_multiply(tDLList *l, unsigned var_number, unsigned label_number)
{
    char *code;
    fillString(&code,
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d$type\n"
            "DEFVAR LF@prec$notype$%d$type\n",
            var_number,var_number+1,var_number+2,
            var_number,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "POPS LF@prec$notype$%d\n"
            "POPS LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n",
            var_number,var_number+1,var_number,var_number,
            var_number+1,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@string\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@string\n"
            "JUMPIFEQ $$equal$types$%d LF@prec$notype$%d$type LF@prec$notype$%d$type\n"
            "JUMPIFEQ $$equal$int$%d LF@prec$notype$%d$type string@int\n"
            "JUMPIFEQ $$equal$float$%d LF@prec$notype$%d$type string@float\n",
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,var_number+1,
            label_number,var_number,label_number,var_number);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "LABEL $$equal$int$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@float\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n"
            "LABEL $$equal$float$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@int\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n" 
            "LABEL $$equal$types$%d\n"
            "MUL LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$end$exp$%d\n"
            "LABEL $$error$types$%d\n"
            "EXIT int@4\n"
            "LABEL $$end$exp$%d\n"
            "PUSHS LF@prec$notype$%d\n",
            label_number,label_number,var_number+1,var_number,var_number,label_number,
            label_number,label_number,var_number+1,var_number+1,var_number+1,label_number,
            label_number,var_number+2,var_number+1,var_number,label_number,
            label_number,label_number,var_number+2);
    DLInsertLast(l,code);
    //gb_free(code);
}

void generate_divide(tDLList *l, unsigned var_number, unsigned label_number)
{
    char *code;
    fillString(&code,
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d$type\n"
            "DEFVAR LF@prec$notype$%d$type\n",
            var_number,var_number+1,var_number+2,
            var_number,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "POPS LF@prec$notype$%d\n"
            "POPS LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n",
            var_number,var_number+1,var_number,var_number,
            var_number+1,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@string\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@string\n"
            "JUMPIFEQ $$equal$types$%d LF@prec$notype$%d$type LF@prec$notype$%d$type\n"
            "JUMPIFEQ $$equal$int$%d LF@prec$notype$%d$type string@int\n"
            "JUMPIFEQ $$equal$float$%d LF@prec$notype$%d$type string@float\n",
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,label_number, var_number+1,
            label_number,var_number,var_number+1,
            label_number,var_number,label_number,var_number);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "LABEL $$equal$int$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@float\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n"
            "LABEL $$equal$float$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@int\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n",
            label_number,label_number,var_number+1,var_number,var_number,
            label_number,label_number,label_number,var_number+1,
            var_number+1,var_number+1,label_number);
    DLInsertLast(l,code);
    //gb_free(code);
    /* IFJcode18 exit status 9 when dividing with zero */
    fillString(&code,
            "LABEL $$equal$types$%d\n"
            "JUMPIFEQ $$equal$types$int$%d LF@prec$notype$%d$type string@int\n"
            "JUMPIFEQ $$error$zero$divide$%d LF@prec$notype$%d float@0x0p+0\n" 
            "DIV LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$end$exp$%d\n"
            "LABEL $$equal$types$int$%d\n"
            "JUMPIFEQ $$error$zero$divide$%d LF@prec$notype$%d int@0\n" 
            "IDIV LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$end$exp$%d\n" 
            "LABEL $$error$types$%d\n"
            "EXIT int@4\n"
            "LABEL $$error$zero$divide$%d\n"
            "EXIT int@9\n"
            "LABEL $$end$exp$%d\n"
            "PUSHS LF@prec$notype$%d\n",
            label_number,label_number,var_number,label_number,var_number,
            var_number+2,var_number+1,var_number,label_number,label_number,
            label_number,var_number,
            var_number+2,var_number+1,var_number,label_number,
            label_number,label_number,label_number,var_number+2);
    DLInsertLast(l,code);
    //gb_free(code);
}

void generate_equality(tDLList *l,unsigned var_number, unsigned label_number,tokenType e)
{
    char *code;
    fillString(&code,
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d$type\n"
            "DEFVAR LF@prec$notype$%d$type\n",
            var_number,var_number+1,var_number+2,
            var_number,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "POPS LF@prec$notype$%d\n"
            "POPS LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n",
            var_number,var_number+1,var_number,var_number,
            var_number+1,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$equal$types$%d LF@prec$notype$%d$type LF@prec$notype$%d$type\n",
            label_number,var_number,label_number,var_number+1,
            label_number,var_number,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "JUMPIFEQ $$equal$int$%d LF@prec$notype$%d$type string@int\n"
            "JUMPIFEQ $$equal$float$%d LF@prec$notype$%d$type string@float\n"
            "LABEL $$equal$int$%d\n"
            "JUMPIFNEQ $$not$equal$%d LF@prec$notype$%d$type string@float\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n"
            "LABEL $$equal$float$%d\n"
            "JUMPIFNEQ $$not$equal$%d LF@prec$notype$%d$type string@int\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n",
            label_number,var_number,label_number,var_number,label_number,
            label_number,var_number+1,var_number,var_number,label_number,
            label_number,label_number,var_number+1, var_number+1,var_number+1,
            label_number);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "LABEL $$not$equal$%d\n"
            "MOVE LF@prec$notype$%d bool@false\n",
            label_number,var_number+2);
    DLInsertLast(l,code);
    //gb_free(code);
    if(e == TYPE_NEG_EQUAL){ // if not equal operator '!='
        fillString(&code,"NOT LF@prec$notype$%d LF@prec$notype$%d\n",
                var_number+2,var_number+2);
        DLInsertLast(l,code);
        //gb_free(code);
    }
    fillString(&code,
            "JUMP $$end$exp$%d\n"
            "LABEL $$equal$types$%d\n"
            "EQ LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n",
            label_number,label_number,var_number+2,var_number+1,
            var_number);
    DLInsertLast(l,code);
    //gb_free(code);
    if(e == TYPE_NEG_EQUAL){ // if not equal operator add this negation
        fillString(&code,"NOT LF@prec$notype$%d LF@prec$notype$%d\n",
                var_number+2,var_number+2);
        DLInsertLast(l,code);
        //gb_free(code);
    }
    fillString(&code,
            "JUMP $$end$exp$%d\n"
            "LABEL $$error$types$%d\n"
            "EXIT int@4\n"
            "LABEL $$end$exp$%d\n"
            "PUSHS LF@prec$notype$%d\n",
            label_number,label_number,label_number,var_number+2);
    DLInsertLast(l,code);
    //gb_free(code);
}

void generate_comparsion(tDLList *l,unsigned var_number, unsigned label_number,tokenType e)
{
    char *code;
    fillString(&code,
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d\n"
            "DEFVAR LF@prec$notype$%d$type\n"
            "DEFVAR LF@prec$notype$%d$type\n",
            var_number,var_number+1,var_number+2,
            var_number,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "POPS LF@prec$notype$%d\n"
            "POPS LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n"
            "TYPE LF@prec$notype$%d$type LF@prec$notype$%d\n",
            var_number,var_number+1,var_number,var_number,
            var_number+1,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    if(e == TYPE_GREAT_EQUAL || e == TYPE_LESS_EQUAL){ // when it's equal comparision we need new variable
        fillString(&code,"DEFVAR LF@prec$notype$%d\n",var_number+3);
        DLInsertLast(l,code);
        //gb_free(code);
    }
    fillString(&code,
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@bool\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$error$types$%d LF@prec$notype$%d$type string@nil\n"
            "JUMPIFEQ $$equal$types$%d LF@prec$notype$%d$type LF@prec$notype$%d$type\n",
            label_number,var_number,label_number,var_number+1,
            label_number,var_number,label_number,var_number+1,
            label_number,var_number,var_number+1);
    DLInsertLast(l,code);
    //gb_free(code);
    fillString(&code,
            "JUMPIFEQ $$equal$int$%d LF@prec$notype$%d$type string@int\n"
            "JUMPIFEQ $$equal$float$%d LF@prec$notype$%d$type string@float\n"
            "LABEL $$equal$int$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@float\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n"
            "LABEL $$equal$float$%d\n"
            "JUMPIFNEQ $$error$types$%d LF@prec$notype$%d$type string@int\n"
            "INT2FLOAT LF@prec$notype$%d LF@prec$notype$%d\n"
            "JUMP $$equal$types$%d\n"
            "LABEL $$equal$types$%d\n",
            label_number,var_number,label_number,var_number,label_number,
            label_number,var_number+1,var_number,var_number,label_number,
            label_number,label_number,var_number+1, var_number+1,var_number+1,
            label_number,label_number);
    DLInsertLast(l,code);
    //gb_free(code);
    if(e == TYPE_LESS || e == TYPE_LESS_EQUAL){ // Make instruction comparsion less
        fillString(&code,"LT LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n",
              var_number+2,var_number+1,var_number);  
        DLInsertLast(l,code);
        //gb_free(code);
    }
    else if(e == TYPE_GREAT || e == TYPE_GREAT_EQUAL){ // great '<'
        fillString(&code,"GT LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n",
              var_number+2,var_number+1,var_number);  
        DLInsertLast(l,code);
        //gb_free(code);
    }

    if(e == TYPE_GREAT_EQUAL || e == TYPE_LESS_EQUAL){ // check if they are equal
        fillString(&code,"EQ LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n"
                         "OR LF@prec$notype$%d LF@prec$notype$%d LF@prec$notype$%d\n",
              var_number+3,var_number+1,var_number,var_number+2,
              var_number+3,var_number+2);  
        DLInsertLast(l,code);
        //gb_free(code);
    }

    fillString(&code,
            "JUMP $$end$exp$%d\n"
            "LABEL $$error$types$%d\n"
            "EXIT int@4\n"
            "LABEL $$end$exp$%d\n"
            "PUSHS LF@prec$notype$%d\n",
            label_number,label_number,label_number,var_number+2);
    DLInsertLast(l,code);
    //gb_free(code);
}


void generate_operation(TEvalStack *t,tDLList *l,htab_t* h)
{
    static htab_t *table = NULL;
    static unsigned var_number= 0; //unique variable number
    static unsigned label_number=0;//unique label number

    if(h != table){ // When local symtable changes, local frame changes as well.
        var_number= 0; //so we start counting variables from beggining
        table = h;
    }

    TStackItemPtr symb2 = t->stack_top;
    TStackItemPtr operator = symb2->prev;
    //TStackItemPtr symb1 = operator->prev;
    switch(operator->token){
        case TYPE_PLUS:
            generate_add(l,var_number,label_number);
            var_number+=3;
            label_number++;
            break;
        case TYPE_MINUS:
            generate_subtract(l,var_number,label_number);
            var_number+=3;
            label_number++;
            break;
        case TYPE_MULT:
            generate_multiply(l,var_number,label_number);
            var_number+=3;
            label_number++;
            break;
        case TYPE_DIV:
            generate_divide(l,var_number,label_number);
            var_number+=3;
            label_number++;
            break;
        case TYPE_EQUAL:
        case TYPE_NEG_EQUAL:
            generate_equality(l,var_number,label_number,operator->token);
            var_number+=3;
            label_number++;
            break;
        case TYPE_GREAT:
        case TYPE_LESS:
            generate_comparsion(l,var_number,label_number,operator->token);
            var_number+=3;
            label_number++;
            break;
        case TYPE_GREAT_EQUAL:
        case TYPE_LESS_EQUAL:
            generate_comparsion(l,var_number,label_number,operator->token);
            var_number+=4;
            label_number++;
            break;
        default:
            gb_exit_process(4);
            break;
    }
}


#define TYPE_NONE -1
void rule_operators(TEvalStack *t,tDLList *l, htab_t *h)
{
    generate_operation(t,l,h);
    es_top_pop(t);
    es_top_pop(t);
    es_top_pop(t);
    es_top_pop(t);

    es_push(t,false,true,TYPE_NONE,NULL,'E');
}

void generate_id(TEvalStack *t,tDLList *l, htab_t *h)
{
   TStackItemPtr term = t->stack_top;
   struct htab_listitem *id;
   char *code;
   char *buffer = NULL;
   switch(term->token){
       case TYPE_ID:
           id = htab_find(h, term->token_attribute);
           if(id == NULL) //id was not defined
               gb_exit_process(3);
           else if(!((TLOCTab*)(id->object))->initialized) gb_exit_process(3);
           else {
               code=gb_malloc(strlen("PUSHS LF@")+strlen(term->token_attribute)+
                       2*sizeof(char));
               strcpy(code,"PUSHS LF@");
               strcat(code,term->token_attribute);
               strcat(code, "\n");
           }
           break;
        case TYPE_INT:
           getVarInt(&buffer, term->token_attribute);
           code = gb_malloc(strlen("PUSHS ") + strlen(buffer) + 2*sizeof(char));
           strcpy(code,"PUSHS ");
           strcat(code,buffer);
            strcat(code, "\n");
           break;
        case TYPE_INT_EXPO:
        case TYPE_FLOAT:
        case TYPE_FLOAT_EXPO:
           getVarFloat(&buffer, term->token_attribute);
           code = gb_malloc(strlen("PUSHS ") + strlen(buffer) + 2*sizeof(char));
           strcpy(code,"PUSHS ");
           strcat(code,buffer);
            strcat(code, "\n");
           break;
        case TYPE_NIL:
           code = gb_malloc(strlen("PUSHS nil@nil") + 2*sizeof(char));
           strcpy(code,"PUSHS nil@nil");
            strcat(code, "\n");
           break;
        case TYPE_QUOT:
        case TYPE_QUOT_EMPTY:
           if(term->token == TYPE_QUOT)
               getVarString(&buffer, term->token_attribute);
           else
               getVarString(&buffer, "");
           code = gb_malloc(strlen("PUSHS ") + strlen(buffer) + 2*sizeof(char));
           strcpy(code,"PUSHS ");
           strcat(code, buffer);
            strcat(code, "\n");
           break;
        default:
           gb_exit_process(3);
   }

    DLInsertLast(l,code);
}

void rule_term_to_noterm(TEvalStack *t,tDLList *l,htab_t *h)
{
    generate_id(t,l,h);
    TStackItem term = es_top_pop(t);
    es_top_pop(t);
    es_push(t,false,true, term.token, term.token_attribute, 'E');
}

void rule_brackets(TEvalStack *t)
{
    es_top_pop(t);
    TStackItem nonterm = es_top_pop(t);
    es_top_pop(t);
    es_top_pop(t);
    
    es_push(t,nonterm.is_terminal,nonterm.is_nonterminal,nonterm.token,
            nonterm.token_attribute,'E');
}
    
