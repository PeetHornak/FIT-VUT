/*
 * This files test symtable and all of its functionality
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/header/symtable.h"

#define HTSIZE 19

void print_global(const char* key, void* object)
{
    TGLOBTab *tmp = object;
    int index = htab_hash_function(key) % HTSIZE;
    printf("%d(Id: %s;%d,%d,%p,%s)\n", index, key, tmp->params_count, tmp->return_type,
            (void*)tmp->loc_symtab,tmp->defined?"true":"false");
}

void print_local(const char* key, void* object)
{
    TLOCTab *tmp = object;
    int index = htab_hash_function(key) % HTSIZE;
    printf("%d(Id: %s;%d,%s)\n", index, key, tmp->type, tmp->initialized?"true":"false");
}

int main()
{
    printf("Symtable test!!!\n");
    printf("Initialization of global table\n");
    htab_t *global_table = htab_init(HTSIZE);
    printf("==============================\n");

    struct htab_listitem *item;

    printf("Adding couple of thing in the table\n");
    //1.
    item = htab_lookup_add(global_table,"Auto",glob_create);
    TGLOBTab *glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 1, INT, NULL, true);
    //2.
    item = htab_lookup_add(global_table,"Veslo",glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 2, INT, NULL, true);
    //3.
    item = htab_lookup_add(global_table,"Postel",glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 3, INT, NULL, true);   
    //4.
    item = htab_lookup_add(global_table,"Bicykel",glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 4, INT, NULL, true);   
    //5.
    item = htab_lookup_add(global_table,"Hojdacka",glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 5, INT, NULL, true);   
    //6.
    item = htab_lookup_add(global_table,"Chodba",glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 6, INT, NULL, true);   
    //7.
    item = htab_lookup_add(global_table,"SmartTV",glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 7, INT, NULL, true);   
    printf("===================================\n");

    printf("Adding to the global table local table\n");
    htab_t *local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table,"Padak",glob_create);
    glob_obj = item->object;
    glob_obj->loc_symtab = local_table;
    printf("======================================\n");


    printf("Adding to the local table couple of things\n");
    htab_lookup_add(glob_obj->loc_symtab,"Premenna1",loc_create);
    htab_lookup_add(glob_obj->loc_symtab,"Premenna2",loc_create);
    htab_lookup_add(glob_obj->loc_symtab,"Premenna3",loc_create);
    htab_lookup_add(glob_obj->loc_symtab,"Premenna4",loc_create);
    htab_lookup_add(glob_obj->loc_symtab,"Premenna5",loc_create);
    htab_lookup_add(glob_obj->loc_symtab,"Premenna6",loc_create);
    htab_lookup_add(glob_obj->loc_symtab,"Premenna7",loc_create);
    htab_lookup_add(glob_obj->loc_symtab,"Premenna8",loc_create);
    htab_lookup_add(glob_obj->loc_symtab,"Premenna9",loc_create);
    printf("=============================================\n");

    printf("Printing Global Table\n");
    htab_foreach(global_table, print_global);
    printf("=============================================\n");

    printf("Printing Local Table\n");
    htab_foreach(glob_obj->loc_symtab, print_local);
    printf("=============================================\n");

    
    printf("Releasing allocated memory of tables\n");
    printf("Local table\n");
    htab_free(glob_obj->loc_symtab);
    printf("Global table\n");
    htab_free(global_table);
    printf("========================================\n");
}
