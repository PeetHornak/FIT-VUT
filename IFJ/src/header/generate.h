#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef _GENERATE_H_
#define _GENERATE_H_

#include "instrlist.h"
#include "generate.h"
#include "symtable.h"
#include "garbagecollector.h"
#include "stack.h"

/* Inicializuje statický stack pre id v if a while pri zanorení */
void myStackInit();

/* Prida na koniec stringu buffer string add
** Ak je buffer NULL alokuje a vloží add */
void addString(char **buffer, char *add);

/* Prida do bufferu vysledok printf nad formatedstring so zadanymi parametrami */
void fillString(char **buffer, char *formatted_string, ...);

/* Konvertuje biely znak na format kompatibilny s IFJcode18 a vloží ho do array */
void convertCharToString(char *array, char dumbchar);

/* Konvertuje zadany buffer na format kompatibilny so zapisom v IFJcode18 */
void convertToString(char **buffer);

/* Vráti v bufferi hodnotu val vo formate int konštanty IFJcode18
** int@val */
void getVarInt(char **buffer,char *val);

/* Vráti v bufferi hodnotu val vo formate float konštanty IFJcode18
** float@val */
void getVarFloat(char **buffer,char *val);

/* Vráti v bufferi hodnotu val vo formate bool konštanty IFJcode18
** bool@val */
void getVarBool(char **buffer,bool value);

/* Vráti v bufferi hodnotu val vo formate string konštanty IFJcode18
** string@val */
void getVarString(char **buffer,char *value);

/* Vytlačí hlavičku .IFJcode18 */
void printHead();


/* FUNKCIE GENERUJÚ INŠTRUKCIE IFJcode18 A UKADAJÚ ICH DO ZOZNAMU L */

/* Priradenie z into do from */
void printAssign(tDLList *L, char *into, char *from);
/* Priradenie vyrazu do premennej name */
void printAssignExpr(tDLList *L,char *name);
/* Priradenie vysledku funkcie */
void printAssignFunc(tDLList *L,char *name);
/* Definovanie premennej */
void printDefine(tDLList *L,char *name);
/* Vlozenie vyhodnoteneho vyrazu do retval */
void printRetval(tDLList *L,char *name);
/* Priradenie vysledku funkcie vo funkcii */
void printFuncReturn(tDLList *L);
/* Generovanie parametrov funkcie podla type */
void printParam(tDLList *L, TYPES type, char *value);
/* Volanie funkcie */
void printCallFunc(tDLList *L, char *name);
/* Priradenie parametrov do premennych s nazvom parametrov v definici */
void printDefParam(tDLList *L, char *name);
/* Nastavy id_param na 1 */
void lastParam();
/* Preddefinovane funkcie */
void printFuncLength(tDLList *L);
void printFuncSubstr(tDLList *L);
void printFuncChr(tDLList *L);
void printFuncOrd(tDLList *L);
void printAllFunc(tDLList *L);
/* Zaciatok a koniec funkcie */
void printFuncBegin(tDLList *L, char *fID);
void printFuncEnd(tDLList *L);
/* Vytvori premennu dump kde uklada vysledok volania inputi/f/s bez priradenia */
void printDumpInput(tDLList *L, TYPES type);
/* Inputi/f/s */
void printFuncInput(tDLList *L, TYPES type, char *name);
/* print */
void printWrite(tDLList *L, TYPES type, char *name);
/* Začiatok a koniec mainu */
void printMainBegin();
void printMainEnd();
/* If a While */
void printElse(tDLList *L);
void printIf(tDLList *L);
void printEndif(tDLList *L);
void printWhile(tDLList *L);
void printEndwhile(tDLList *L);

#endif
