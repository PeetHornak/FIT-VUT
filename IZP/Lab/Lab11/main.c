/**
Projekt:    Kostra 9. cviceni IZP 2015
Autor:      Marek Zak <izakmarek@fit.vutbr.cz>
Datum:      28. 11. 2015
*/

#include <stdio.h>
#include "struct.h"
#include "array.h"

/**
 * funkce main
 */
int main()
{
    /** ********* testovani funkci struktury ********** */

    printf("========== STRUCT CHECK ==========\n\n");


//  vytvoreni objektu

    char tmp_name[] = "Hello 1";
    Object o1 = object_ctor(1, tmp_name);
    Object o2 = object_ctor(2, "Hello 2");
    tmp_name[6] = '?';

    printf("INIT OBJECTS\n");
    print_object(&o1);
    print_object(&o2);


//  zamena objektu

    printf("\nSWAP\n");
    object_swap(&o1, &o2);

    print_object(&o1);
    print_object(&o2);


//    kopirovani objektu

    printf("\nCOPY\n");
    Object copy;
    print_object(item_cpy(&copy, &o1));


//    kopirovani prazdneho objektu
    printf("\nCOPY EMPTY\n");
    object_dtor(&copy);
    o1.id = -1;
    free(o1.name);

    o1.name = NULL;
    item_cpy(&copy, &o1);

    print_object(&copy);


//    smazani objektu

    object_dtor(&o1);
    object_dtor(&o2);
    object_dtor(&copy);




    /** ********* testovani funkci pole ********** */

    printf("\n\n========== ARRAY CHECK ==========\n\n");


    /** vytvori pole velikosti ARRAY_SIZE */

    Array a = array_ctor(ARRAY_SIZE);

    printf("INIT ARRAY\n");
    print_array_size(a, ARRAY_SIZE);

    /** zmensi velikost pole */

    array_resize(&a, SMALLER_ARRAY_SIZE);

    printf("\n\nSMALLER ARRAY\n");

    if (a.size == SMALLER_ARRAY_SIZE)
    {
        print_array_size(a, SMALLER_ARRAY_SIZE);
    }
    else
    {
        fprintf(stderr, "ERROR: Array does not contains %d Object(s).\n", SMALLER_ARRAY_SIZE);
    }

    /** zvetsi velikost pole */

    /*array_resize(&a, BIGGER_ARRAY_SIZE);

    printf("\n\nBIGGER ARRAY\n");

    if (a.size == BIGGER_ARRAY_SIZE)
    {
        print_array_size(a, BIGGER_ARRAY_SIZE);
    }
    else
    {
        fprintf(stderr, "ERROR: Array does not contains %d Object(s).\n", BIGGER_ARRAY_SIZE);
    }*/

    /** vyhledani id v poli */

    /*a.items[SEARCH_ARR_ID_POS].id = SEARCH_ARR_ID;

    printf("\n\nSEARCH ARRAY ID\n");

    int fi = array_find_id(&a, SEARCH_ARR_ID);
    printf("ID POSITION: %d, SHOULD BE %d\n", fi, SEARCH_ARR_ID_POS);*/


    // vyhledani name v poli
    /*a.items[SEARCH_ARR_NAME_POS].name = malloc(strlen(SEARCH_ARR_NAME) + 1);
    strcpy(a.items[SEARCH_ARR_NAME_POS].name, SEARCH_ARR_NAME);

    print_object(&a.items[SEARCH_ARR_NAME_POS]);

    printf("\n\nSEARCH ARRAY NAME\n");

    int fn = array_find_name(&a, SEARCH_ARR_NAME);
    printf("NAME POSITION: %d, SHOULD BE %d\n", fn, SEARCH_ARR_NAME_POS);

	print_array(a);*/

    /** vlozeni prvku na zadanou pozici */
    /*Object new_item = object_ctor(ADD_ARRAY_ID, ADD_ARRAY_NAME);

    a.items[ADD_ARRAY_POS].id = ACT_ARRAY_ID;
    a.items[ADD_ARRAY_POS].name = realloc(a.items[ADD_ARRAY_POS].name,strlen(ACT_ARRAY_NAME) + 1);
    strcpy(a.items[ADD_ARRAY_POS].name, ACT_ARRAY_NAME);

    array_insert_item(&a, &new_item, ADD_ARRAY_POS);

    printf("\n\nADD TO ARRAY ON POSITION\n");

    if (a.items[ADD_ARRAY_POS].id == ADD_ARRAY_ID &&
        strcmp(a.items[ADD_ARRAY_POS].name, ADD_ARRAY_NAME) == 0 &&
        a.items[ADD_ARRAY_POS + 1].id == ACT_ARRAY_ID &&
        strcmp(a.items[ADD_ARRAY_POS + 1].name, ACT_ARRAY_NAME) == 0)
    {
        printf("Object {%d, '%s'} added to position %d\n", ADD_ARRAY_ID, ADD_ARRAY_NAME, ADD_ARRAY_POS);
    }
    else
    {
        fprintf(stderr, "ERROR: Object added unsuccessfully.\n");
    }

	array_dtor(&a);*/

    /** nalezeni minimalniho prvku */

    /*Array b = array_ctor(BIGGER_ARRAY_SIZE);

    for (unsigned int i = 0; i < b.size; i++)
    {
        b.items[i].id = (i * 5 + 7) % BIGGER_ARRAY_SIZE;
    }

    printf("\n\nMIN ITEM POSITION\n");

    print_array(b);

    printf("MINIMUM: %d\n", array_find_min(&b,0));*/


    /** serazeni pole */

    /*printf("\n\nSORT ARRAY ITEMS\n");

    array_sort(&b);

    print_array(b);

	array_dtor(&b);*/


    return 0;
}
