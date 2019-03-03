/*
 * Subor: htab_hash_function.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Hashovacia funkcia
 */

#include "htab.h"

unsigned htab_hash_function(const char *str){
    unsigned h = 0;
    const unsigned char *p;
    for(p = (const unsigned char*)str; *p != '\0'; p++)
        h = 65599*h + *p;
    return h;
}
