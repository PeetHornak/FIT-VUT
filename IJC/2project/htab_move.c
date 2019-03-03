/*
 * Subor: htab_move.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Vytvorenie a inicializacia tabulky na zaklade dat z tabulky t2, t2 zostane prazdna
 */

#include "htab.h"

htab_t* htab_move(unsigned newsize, htab_t* t2){
    
    htab_t *t = htab_init(newsize);

    if(t2 == NULL || t == NULL)
        return NULL;

    struct htab_listitem* item = NULL;
    struct htab_listitem* temp = NULL;
    
    //Preiteruje cez kazdy zoznam
    for(unsigned i = 0; i < htab_bucket_count(t2); i++){
        item = t2->arr[i];
        //Iteracia zoznamom prida slovo do noveho zoznamu, skopiruje data
        while(item != NULL){
            temp = htab_lookup_add(t, item->key);
            temp->data = item->data;
            item = item->next;
            t->size++;
        }
    }
    //Vycisti zoznamy starej tabulky
    htab_clear(t2);
    return t;
}
