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


#ifndef _GARBAGE_H_
#define _GARBAGE_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#define TRUE 1
#define FALSE 0
                                           /* Indikace, zda byl příklad řešen */
                                   /* Detects whether the task is implemented */
extern int solved;
                                                            /* Indikace chyby */
                                                                /* Error flag */
extern int errflg;
                                                             /* Prvek seznamu */
                                                /* Definition of list element */
typedef struct tElem {
    struct tElem *ptr;
    void *allocated_buffer;
} *tElemPtr;
                                                            /* Vlastní seznam */
                                                        /* Definition of list */
typedef struct {
    tElemPtr Act;
    tElemPtr First;
} tList;



/*Garbage collector Interface*/

/**
 * This function should be called beforehand each garbage collector function.
 */
void garbage_collector_init();
/**
 * This function registers newpointer to garbagecollector so it can be free at the end
 * @param pointer Pointer to void which should be registered.
 */
void gb_register_pointer(void*);
/**
 * Same as malloc except it also register pointer which was returned by malloc.
 * @param size Size of the memory which is going to be allocated by malloc.
 * @return Void pointer which was returned by malloc or Null on failure.
 */
void* gb_malloc(size_t size);
/**
 * Same as realloc, also reregisters pointer in garbage collector if needed.
 * @param allocated_mem Pointer which should be reallocated.
 * @return New void pointer.
 */
void* gb_realloc(void* allocated_mem,size_t);
/**
 * Unregisters pointer from garbage collector if was registered and also frees the memory.
 */
void gb_free(void*);
/**
 * Function releases all resources allocated by garbage collector functions.
 */
void gb_release_resources();
/**
 * Function exit with int value parameter and beforehand releases resources.
 */
void gb_exit_process(int);

                                                   /* Funkce pro implementaci */
void InitList ();
void DisposeList ();
void InsertFirst (void*);
void First ();
void Succ ();
void* Copy ();	
void Actualize (void*);
int  Active ();
void DeleteActive();

/* Konec hlavičkového souboru c201.h */

#endif
