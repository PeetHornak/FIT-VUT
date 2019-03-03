/*
 * Subor: htab_free.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Uvolni alokovanu pamat pre tabulku
 */

#include "htab.h"

void htab_free(htab_t* t){
    
    if(t == NULL)
        return;

    htab_clear(t);
    free(t);
    t = NULL;
}
