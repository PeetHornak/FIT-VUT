/** garbagecollector.c
 * This file implements memory operations which are handled in
 * one-directional list and it releases all memory resources when program
 * finishes.
 * Author: Nikolas Patrik xpatri00
 * Collaborators:
 *      Peter Hornak   xhorna14
 *      Matej Jancek   xjance00
 *      Robert Hubinak xhubin03
 *
 */
#include <stdlib.h>
#include <stdbool.h>

#include "header/garbagecollector.h"

int errflg;
int solved;
tList L;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList ()
{
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L.Act = NULL;
    L.First = NULL;
}

void DisposeList ()
{
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
    for(tElemPtr i = L.First; i !=NULL;){
       tElemPtr tmp = i; // Uchovaj staru polozku seznamu
       i = i->ptr;
       free(tmp->allocated_buffer);
       free(tmp); //Uvolni tuto polozku
    }

    L.Act = NULL;
    L.First = NULL;
}

bool is_already_inside(void *memory_pointer)
{
    for(First(); Active(); Succ()){
        if(L.Act->allocated_buffer == memory_pointer)
            return true;
    }
    return false;
}

void InsertFirst (void* memory_pointer) 
{
/*
** Vloží prvek s hodnotou val na začátek seznamu L ak sa prvok uz nachadza
** v sezname tak nedela nic..
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    if(is_already_inside(memory_pointer))
        return;

    tElemPtr tmp = malloc(sizeof(struct tElem));
    if(tmp == NULL){
        Error();
        return;
    }

    //Incializuj polozku
    tmp->allocated_buffer = memory_pointer;
    tmp->ptr = L.First;

    //Vlozenie na zaciatok seznamu
    L.First = tmp;
}

void First () 
{
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
    L.Act = L.First;
}

void* Copy () {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/	
    if(L.Act == NULL){
        Error();
        return NULL;
    }
    return L.Act->allocated_buffer;
}

void Actualize (void* mem)
{
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
    if(L.Act == NULL)
        return;

    L.Act->allocated_buffer = mem;	
}

void Succ ()
{
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
    if(L.Act == NULL)
        return;
    L.Act = L.Act->ptr;
}

int Active () 
{
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
    return L.Act ? 1 : 0;
}

void DeleteActive()
{
    if(!Active())
        return;
    for(tElemPtr i = L.First; i != NULL; i= i->ptr){
        if(i->ptr == L.Act){
            i->ptr = L.Act->ptr;
            free(L.Act->allocated_buffer);
            free(L.Act);
        }
    }
}

void garbage_collector_init()
{
    InitList();
}

void gb_register_pointer(void* pointer)
{
    InsertFirst(pointer);
}

void* gb_malloc(size_t size)
{
    void* item = malloc(size);
    if(item == NULL) //If Null dont insert inside list
        return item;

    InsertFirst(item);
    return item;
}

void* gb_realloc(void *allocated_mem, size_t new_size)
{
   if(!is_already_inside(allocated_mem))
       First();

   void *new = realloc(allocated_mem,new_size);
   Actualize(new);
   return new;
}

void gb_free(void* allocated_mem)
{
    for(First();Active();Succ()){
        if(Copy() == allocated_mem)
            DeleteActive();
    }
}

void gb_release_resources()
{
    DisposeList();
}

void gb_exit_process(int exit_code)
{
    gb_release_resources();
    exit(exit_code);
}
