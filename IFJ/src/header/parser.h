#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"
#include "parseexp.h"
#include "garbagecollector.h"
#include "instrlist.h"
#include "generate.h"

#ifndef _PARSER_H_
#define _PARSER_H_

bool parse();

bool st_list(tokenType*, char**);

bool stat(tokenType*, char**);

bool else_st_list(tokenType*, char**);

bool end_st_list(tokenType*, char**);

bool id_item(tokenType*, char**);

bool assign(tokenType*, char**, char*);

bool next(tokenType*, char**, char*);

bool func(tokenType*, char**);

bool param(tokenType*, char**);

bool next_param(tokenType*, char**);

bool bracket(tokenType*, char**);

bool brc_param(tokenType*, char**);

bool next_brc_param(tokenType*, char**);

bool expr(tokenType*, char**);

void checkWhile();

void checkIf();

#endif
