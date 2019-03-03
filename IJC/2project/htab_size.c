/*
 * Subor: htab_size.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Vracia pocet prvkov zoznamu
 */

#include "htab.h"

unsigned htab_size(htab_t* t){
    return t->size;
}
