/*
 * Subor: htab_clear.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Vymaze zoznam a jeho prvky
 */

#include "htab.h"

void htab_clear(htab_t* t){
    
    if(t == NULL)
        return;
    
    struct htab_listitem* item = NULL;
    struct htab_listitem* temp = NULL;

    // Prejde kazdy zoznam
    for(unsigned i = 0; i < htab_bucket_count(t); i++){
        item = t->arr[i];
        // Prejde zoznam az po koniec a uvolni ho
        while(item != NULL){
            temp = item->next;
            free(item->key);
            free(item);
            t->size--;
            item = temp;
        }
        t->arr[i] = NULL;
        item = NULL;
    }
}
