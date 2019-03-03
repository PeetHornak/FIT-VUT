/*
 * Subor: htab.h
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Hlavickovy subor hashovacej tabulky
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct htab_listitem{
    char* key;
    unsigned data;
    struct htab_listitem *next;
};

typedef struct hashtable{
    unsigned arr_size;
    unsigned size;
    struct htab_listitem* arr[];
} htab_t;

unsigned htab_hash_function(const char *str);

htab_t* htab_init(unsigned arr_size);

htab_t* htab_move(unsigned newsize, htab_t* t2);

unsigned htab_size(htab_t* t); //size

unsigned htab_bucket_count(htab_t* t); //arr_size

struct htab_listitem* htab_lookup_add(htab_t *t, const char* key);

struct htab_listitem* htab_find(htab_t *t, const char* key);

void htab_foreach(htab_t* t, void (*func)(const char *, unsigned *));

bool htab_remove(htab_t* t, const char* key);

void htab_clear(htab_t* t);

void htab_free(htab_t* t);
