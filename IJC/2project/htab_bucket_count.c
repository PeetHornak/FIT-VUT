/*
 * Subor: htab_bucket_count.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Vrati pocet zoznamov v tabulke
 */

#include "htab.h"

unsigned htab_bucket_count(htab_t* t){
    return t->arr_size;
}
