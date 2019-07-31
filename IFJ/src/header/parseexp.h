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

#ifndef _PARSE_EXP_H_
#define _PARSE_EXP_H_

#define TERM_COUNT 14

#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"
#include "instrlist.h"

typedef struct stack_item {
    bool is_terminal;
    bool is_nonterminal;
    tokenType token;
    char *token_attribute;
    char info;
    struct stack_item *prev;
}TStackItem;

typedef TStackItem *TStackItemPtr;


typedef struct eval_stack {
    struct stack_item *stack_top;
    unsigned count;
}TEvalStack;

/**
 * This function checks the syntax of every expression in source code and simultaneously generates code in
 * IFJcode18.
 * @param end Type of the token when analysis should ended.
 * @param end_token_attribute Attribute of the ending token.
 * @param symtab Local symtable which handled local variable for semantic analysis whether var is defined.
 * @param l_main Bi-directional list which contains all instructions of program.
 * @return True or false whether syntax is correct or not.
 */
bool expression_parse(tokenType end, char* end_token_attribute, htab_t *symtab, 
        tDLList *l_main);

#endif
