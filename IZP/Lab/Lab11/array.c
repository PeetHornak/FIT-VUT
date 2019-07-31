/**
Projekt:    Kostra 9. cviceni IZP 2015
Autor:      Marek Zak <izakmarek@fit.vutbr.cz>
Datum:      28. 11. 2015
*/

#include "array.h"

/**
 * Konstruktor pole. Vytvoří pole o velikosti size a každý prvek
 * v něm inicializuje na hodnoty 0, NULL.
 */
Array array_ctor(unsigned size)
{
	Array a;
	a.size = size;
	a.items = malloc(sizeof(Object) * size);

	if (a.items!=NULL){
		for(unsigned i = 0; i < size; i++){
			a.items[i] = object_ctor(0, NULL);
		}
	}
	return a;

}

/**
 * Uvolní alokované místo pro pole (a každý jeho objekt) z paměti.
 */
void array_dtor(Array *arr)
{
	for(unsigned i = 0; i < arr->size; i++){
		object_dtor(&arr->items[i]);
	}

	free(arr->items);
}

/**
 * Změna velikosti pole. Změní/realokuje zadané pole na novou velikost.
 * V případě zvětšení ještě inicializuje každý prvek na hodnoty 0, NULL
 */
Array array_resize(Array *arr, unsigned size)
{
	if(arr->items!=NULL){
		if(size > arr->size){
			for(unsigned i = arr->size-1; i <size; i++)
				arr->items[i] = object_ctor(0, NULL);
		}
		
		arr->size = size;
	}
	
	return *arr;
}

/**
 * Hledání prvku v poli podle identifikátoru prvku. Vrací index prvku v poli
 * nebo -1, pokud prvek pole neobsahuje.
 */
int array_find_id(Array *arr, int id)
{
	for(unsigned i = 0; i < arr->size; i++){
		if(arr->items[i].id == id)
			return i;
	}
	return -1;
}

/**
 * Hledání prvku v poli podle názvu. Vrací index prvku v poli
 * nebo -1, pokud prvek pole neobsahuje.
 */
int array_find_name(Array *arr, char *name)
{
	for(unsigned i = 0; i < arr->size; i++){
		if(arr->items[i].name != NULL && !strcmp(arr->items[i].name, name)
			return i;
	}
	return -1;

}

/**
 * Vložení prvku do pole na zadaný index. Vrací index vloženého prvku (idx)
 * nebo -1, pokud se operace nezdarila.
 */
int array_insert_item(Array *arr, Object *item, unsigned idx)
{
	/** TODO */
}

/**
 * Hledání prvku s nejmenším identifikátorem. Vrací index prvku nebo -1,
 * pokud je pole prázdné.
 * Promenna l urcuje index, od ktereho se bude pole prohledavat.
 */
int array_find_min(Array *arr, unsigned l)
{
	/** TODO */
	
}

/**
 * Øazení prvkù v poli podle jejich identifikátorù.
 */
void array_sort(Array *arr)
{
	/** TODO */
}

/**
 * Vypise prvky pole.
 */
void print_array(Array a)
{
    for (unsigned int i = 0; i < a.size; i++)
    {
        printf("#%d\t", i);
        print_object(&a.items[i]);
    }
    printf("\n");
}

/**
 * Vypise prvky pole dane velikosti.
 */
void print_array_size(Array a, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        printf("#%d\t", i);
        print_object(&a.items[i]);
    }
    printf("\n");
}
