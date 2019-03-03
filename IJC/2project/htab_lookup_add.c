/*
 * Subor: htab_lookup_add.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Hlada slovo v tabulke, ak ho najde zvacsi data, inak ho vlozi do tabulky
 */

#include "htab.h"

struct htab_listitem* htab_lookup_add(htab_t* t, const char* key){
    
    if(t == NULL || key == NULL)
        return NULL;

    //Index vypocitany cez hash modulo arr_size
    unsigned i = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_listitem* item = t->arr[i];
    struct htab_listitem* previous = NULL;
        
    //Cyklus ktory hlada zhodu, pri zhode inc data a returne item
    while(item != NULL){
        if(!strcmp(key, item->key)){
            item->data++;
            return item;
        }
        previous = item;
        item = item->next;
    }

    //Inak vytvor novy item
    struct htab_listitem* new_item = (struct htab_listitem*) malloc(sizeof(struct htab_listitem));
        
    if(new_item == NULL)
        return NULL;

    //Alokacia pamate pre key a +1 byte pre ukoncovaciu nulu    
    new_item->key = (char*) malloc(sizeof(char)*(strlen(key)+1));
    
    if(new_item->key == NULL){
        free(new_item);
        return NULL;
    }
    //Nastavenie novych dat itemu
    strcpy(new_item->key, key);
    new_item->next = NULL;
    new_item->data = 1;
    t->size++;

    if(previous == NULL){
        t->arr[i] = new_item;
    }
    else{
        previous->next = new_item;
    }

    return new_item;
}
