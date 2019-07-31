/**
Projekt:    Kostra 9. cviceni IZP 2015
Autor:      Marek Zak <izakmarek@fit.vutbr.cz>
Datum:      28. 11. 2015
*/

#include "struct.h"

/**
 * Inizializace objektu. Název objektu kopíruje. Objekt bude mít název
 * roven NULL, pokud se inicializace nezdařila.
 */
Object object_ctor(int id, char *name)
{
	Object o;
	o.id = id;

	if (name != NULL) {
		o.name = (char *) malloc(sizeof(char)*(strlen(name)+1));
	}
	else
		o.name = NULL;

	if (name !=NULL)
		strcpy(o.name, name);

	return o;
}

/**
 * Záměna dat dvou objektù.
 */
void object_swap(Object *i1, Object *i2)
{
	Object tmp = *i1;
	*i1 = *i2;
	*i2 = tmp;
}

/**
 * Hluboká kopie objektu src na dst. Vrací dst, pokud se operace povedla,
 * jinak NULL.
 */
Object *item_cpy(Object *dst, Object *src)
{
	if (src != NULL){
		*dst = object_ctor(src->id,src->name );
	}
	else{
		*dst = object_ctor(src->id,src->name );
	}
	return dst;
}

/**
 * Uvolní objekt (resp. jeho jméno) z paměti. 
 * Nastavi jmeno objektu na NULL.
 */
void object_dtor(Object *o)
{
		free(o->name);
		o->name = NULL;
}

/**
 * Vytiske hodnoty daneho objektu.
 */
void print_object(Object *o)
{
    printf("ID: %d, NAME: %s\n", o->id, o->name);
}
