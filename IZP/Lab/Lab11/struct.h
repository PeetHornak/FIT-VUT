/**
Projekt:    Kostra 9. cviceni IZP 2015
Autor:      Marek Zak <izakmarek@fit.vutbr.cz>
Datum:      28. 11. 2015
*/

#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * struktura Object
 */
typedef struct {
  int id;
  char *name;
} Object;

/**
 * Inizializace objektu. Název objektu kopíruje. Objekt bude mít název
 * roven NULL, pokud se inicializace nezdařila.
 */
Object object_ctor(int id, char *name);

/**
 * Záměna dat dvou objektů.
 */
void object_swap(Object *i1, Object *i2);

/**
 * Hluboká kopie objektu src na dst. Vrací dst, pokud se operace povedla,
 * jinak NULL.
 */
Object *item_cpy(Object *dst, Object *src);

/**
 * Uvolní objekt (resp. jeho jméno) z paměti.
 */
void object_dtor(Object *o);

/**
 * Vytiske hodnoty daneho objektu.
 */
void print_object(Object *o);

#endif // STRUCT_H_INCLUDED
