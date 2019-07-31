/**
Projekt:    Kostra 9. cviceni IZP 2015
Autor:      Marek Zak <izakmarek@fit.vutbr.cz>
Datum:      28. 11. 2015
*/

#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

#define ARRAY_SIZE          5
#define BIGGER_ARRAY_SIZE   8
#define SMALLER_ARRAY_SIZE  3

#define SEARCH_ARR_ID       99
#define SEARCH_ARR_ID_POS   4

#define SEARCH_ARR_NAME_POS 3
#define SEARCH_ARR_NAME     "IZP 2017"

#define ADD_ARRAY_POS       3
#define ACT_ARRAY_ID        95
#define ACT_ARRAY_NAME      "Old One"
#define ADD_ARRAY_ID        98
#define ADD_ARRAY_NAME      "New Added"

typedef struct {
  unsigned int size;
  Object *items;
} Array;

/**
 * Konstruktor pole. Vytvoří pole o velikosti size a každý prvek
 * v něm inicializuje na hodnoty 0, NULL.
 */
Array array_ctor(unsigned size);

/**
 * Uvolní alokované místo pro pole (a každý jeho objekt) z paměti.
 */
void array_dtor(Array *arr);

/**
 * Změna velikosti pole. Změní/realokuje zadané pole na novou velikost.
 * V případì zvětšení ještì inicializuje každý prvek na hodnoty 0, NULL
 */
Array array_resize(Array *arr, unsigned size);

/**
 * Hledání prvku v poli podle identifikátoru prvku. Vrací index prvku v poli
 * nebo -1, pokud prvek pole neobsahuje.
 */
int array_find_id(Array *arr, int id);

/**
 * Hledání prvku v poli podle názvu. Vrací index prvku v poli
 * nebo -1, pokud prvek pole neobsahuje.
 */
int array_find_name(Array *arr, char *name);

/**
 * Vložení prvku do pole na zadaný index. Vrací index vloženého prvku (idx)
 * nebo -1, pokud se operace nezdařila.
 */
int array_insert_item(Array *arr, Object *item, unsigned idx);

/**
 * Hledání prvku s nejmenším identifikátorem. Vrací index prvku nebo -1,
 * pokud je pole prázdné.
 * Promenna l urcuje index, od ktereho se bude pole prohledavat.
 */
int array_find_min(Array *arr, unsigned l);

/**
 * Řazení prvků v poli podle jejich identifikátorů.
 */
void array_sort(Array *arr);

/**
 * Vypise prvky pole.
 */
void print_array(Array a);

/**
 * Vypise prvky pole dane velikosti.
 */
void print_array_size(Array a, unsigned int size);

#endif // ARRAY_H_INCLUDED
