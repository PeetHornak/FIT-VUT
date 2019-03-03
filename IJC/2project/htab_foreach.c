/*
 * Subor: htab_foreach.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Zavola funkciu pre kazdy prvok tabulky
 */

#include "htab.h"

void htab_foreach(htab_t* t, void(*func)(const char *, unsigned *)){
    
    if(t == NULL)
        return;

    struct htab_listitem* item;

    //Iteruj cez kazdy zoznam
    for(unsigned i = 0; i < htab_bucket_count(t); i++){
        if(t->arr[i] != NULL){
            item = t->arr[i];
            //Iteruj cez zoznam
            while(item != NULL){
                func(item->key, &item->data); //Zavolaj funkciu pre kazdy item
                item = item->next;
            }
        }
    }
}
