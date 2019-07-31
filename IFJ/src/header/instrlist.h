#ifndef _INSTRLIST_H_
#define _INSTRLIST_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "garbagecollector.h"

typedef struct tDLElem {                // Prvok zoznamu
        char *instruction;              // Inštrukcia(e)
        struct tDLElem *lptr;           // Ukazatel na predchadzajúci prvok
        struct tDLElem *rptr;           // Ukazatel na nasledujúcí prvok
} *tDLElemPtr;

typedef struct {                        // Zoznam
    tDLElemPtr First;                   // Ukazatel na prvý prvok
    tDLElemPtr Act;                     // Ukazatel na aktuálny prvok
    tDLElemPtr Last;                    // Ukazatel na posledný prvok
} tDLList;

/* Inicializuje zoznam, tým že nastaví ukazatele na NULL */                                             /* prototypy jednotlivých funkcí */
void DLInitList (tDLList *);

/* Zruší zoznam a uvolní pamäť prvku a inšrukcie volaním gb_free
** Zoznam nastaví do stavu po inicializácii */
void DLDisposeList (tDLList *);

/* Vloží zoznam M do zoznamu L za Act prvok
** V prípade že zoznamy niesú inicializovane
** alebo L nieje aktívny volá DLError */
void DLPostInsertList (tDLList *L, tDLList *M);

/* Vloží zoznam M do zoznamu L pred Act prvok
** V prípade že zoznamy niesú inicializovane
** alebo L nieje aktívny volá DLError */
void DLPreInsertList (tDLList *L, tDLList *M);

/* Vloží prvok s inštrukciou na začiatok zoznamu
** Pri chybe alokacie zoznamu alebo inštrukcie vola gb_exit_process */
void DLInsertFirst (tDLList *, char *);

/* Vloží prvok s inštrukciou na koniec zoznamu
** Pri chybe alokacie listu alebo inštrukcie vola gb_exit_process */
void DLInsertLast(tDLList *, char *);

/* Nastaví aktivitu na prvý prvok zoznamu */
void DLFirst (tDLList *);

/* Nastaví aktivitu na posledný prvok zoznamu */
void DLLast (tDLList *);

/* Prostredníctvom parametru val vráti inštrukciu
** uloženú na prvom prvku zoznamu. Pokial je zoznamu
** prázdny volá DLError */
void DLCopyFirst (tDLList *L, char **val);

/* Prostredníctvom parametru val vráti inštrukciu
** uloženú na poslednom prvku zoznamu. Pokial je zoznamu
** prázdny volá DLError */
void DLCopyLast (tDLList *L, char **val);

/* Zruší a uvolní prvý prvok zoznamu.
** Ak je prázdny, nič nerobí */
void DLDeleteFirst (tDLList *);

/* Zruší a uvolní posledný prvok zoznamu.
** Ak je prázdny, nič nerobí */
void DLDeleteLast (tDLList *);

/* Zruší a uvolní prvok za Act prvkom zoznamu.
** Ak je prázdny alebo Act == Last nič nerobí */
void DLPostDelete (tDLList *);

/* Zruší a uvolní prvok pred Act prvkom zoznamu.
** Ak je prázdny alebo Act == Last nič nerobí */
void DLPreDelete (tDLList *);

/* Vloží prvok s inštrukciou za Act prvok zoznamu.
** Ak zoznam nie je aktívny, nič nerobí */
void DLPostInsert (tDLList *, char *);

/* Vloží prvok s inštrukciou pred Act prvok zoznamu.
** Ak zoznam nie je aktívny, nič nerobí */
void DLPreInsert (tDLList *, char *);

/* Prostredníctvom parametru val vráti inštrukciu
** uloženú na aktívnom prvku zoznamu. Pokial je zoznamu
** prázdny volá DLError */
void DLCopy (tDLList *L, char **val);

/* Prepíše inštrukciu na aktívnom prvku za val.
** Ak zoznam nie je aktívny, nič nerobí */
void DLActualize (tDLList *L, char *val);

/* Posunie aktivitu na nadchádzajúci prvok */
void DLSucc (tDLList *);

/* Posunie aktivitu na predchádzajúci prvok */
void DLPred (tDLList *);

/* Vráti 1 ak je zoznam aktívny, inak 0 */
int DLActive (tDLList *);

/* Vypíše inštrukcie zoznamu */
void DLPrintList (tDLList *);

#endif
