/*
 * Subor: htab_remove.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Vyhlada a vymaze zadane slovo z tabulky
 */

#include "htab.h"

bool htab_remove(htab_t* t, const char* key){
    
    if(t == NULL || key == NULL)
        return false;

    unsigned i = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_listitem* item = t->arr[i];
    struct htab_listitem* previous = NULL;
    
    while(item != NULL){
        if(!strcmp(key, item->key)){ //Ak zhoda
            free(item->key); // Uvolni key
            t->size--; // Zmensi pocet itemov
            if(previous == NULL) //Ak je prvy, na zaciatok prirad next
                t->arr[i]=item->next; 
            else
                previous->next = item->next; //Inak previous prirad next
            free(item); //Uvolni item
            return true;
        }
        previous = item;
        item = item->next;
    }
    return false;
}
