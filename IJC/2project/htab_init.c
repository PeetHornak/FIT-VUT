/*
 * Subor: htab_init.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Inicializacia tabulky o danej velkosti
 */

#include "htab.h"

htab_t* htab_init(unsigned arr_size){
    //Alokacia- velkost tabulky + velkost (size) itemov
    htab_t* htab = (htab_t *) malloc(sizeof(htab_t) + arr_size*sizeof(struct htab_listitem *));

    if(htab == NULL)
        return NULL; // Zla alokacia
    
    htab->arr_size = arr_size;
    //Prvy prvok kazdeho zoznamu ukazuje na NULL
    for(unsigned i = 0; i < arr_size; i++)
        htab->arr[i] = NULL;

    return htab;
}
