/*
 * Subor: htab_find.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Hlada slovo v tabulke, ak najde vrati pointer, inak NULL
 */

#include "htab.h"

struct htab_listitem* htab_find(htab_t* t, const char* key){
    
    if(t == NULL || key == NULL)
        return NULL;
    
    //Index vypocitany cez hash modulo arr_size
    unsigned i = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_listitem *item = t->arr[i];

    //Cyklus ktory hlada zhodu
    while(item != NULL){
        if(!strcmp(key, item->key))
            break;
        item = item->next;
    }
    return item;
}
