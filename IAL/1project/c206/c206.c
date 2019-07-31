
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
 
    L->Act = L->First = L->Last = NULL;     // Inicializacia na NULL
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    
    while(L->First != NULL){            // Iteracia cez cely zoznam 
        L->Act = L->First->rptr;        
        free(L->First);
        L->First = L->Act;              // Z nasledujuceho prvku sa stava prvy
    }   
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    
    tDLElemPtr new;
    
    if((new = malloc(sizeof(struct tDLElem))) == NULL){     // Kontrola spravnej alokacie
        DLError();
        return;
    }    

    new->data = val;
    new->rptr = L->First;
    new->lptr = NULL;

    if(L->First != NULL)            // Ak nie je zoznam prazdny
        L->First->lptr = new;       // Predchadzajuci prvok prirad novy
    else
        L->Last = new;              // Ak je prazdny novy prvok je zaroven posledny
    L->First = new;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	
/* Taky isty postup ako pri DLInsertFirst */

    tDLElemPtr new;
    
    if((new = malloc(sizeof(struct tDLElem))) == NULL){ 
        DLError();
        return;
    }    
    
    new->data = val;
    new->rptr = NULL;
    new->lptr = L->Last;
    if(L->Last != NULL)
        L->Last->rptr = new;
    else
        L->First = new;
    L->Last = new;

}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->First;    

}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if(L->First == NULL){   // Kontrola prazdneho zoznamu
        DLError();
        return;
    }

    *val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
    if(L->Last == NULL){    //  Kontrola prazdneho zoznamu
        DLError();
        return;
    }
	
    *val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
    if(L->First == NULL)    // Kontrola prazdneho zoznamu
        return;
    
    if(L->Last == L->First){    // Ak je v zozname jeden prvok, uvolni ho
        free(L->First);
        L->First = L->Last = L->Act = NULL;
        return;
    }

    tDLElemPtr temp = L->First->rptr;       // Priradenie nasledujuceho prvku do temp
        temp->lptr = NULL;              // Priradenie NULL na miesto kde sa bude mazat prvok
    if(L->First == L->Act)          // Zrusenie aktivity ak je na prvom prvku
        L->Act = NULL;
    free(L->First);
    L->First = temp;                // Nastavenie nasledujuceho prvku na prvy
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	
/* Rovnaky sposob ako pri DLDeleteFirst */

    if(L->Last == NULL)
        return;

    if(L->Last == L->First){
        free(L->Last);
        L->First = L->Last = L->Act = NULL;
        return;
    }

    tDLElemPtr temp = L->Last->lptr;
    temp->rptr = NULL;
    if(L->Last == L->Act)
        L->Act = NULL;
    free(L->Last);
    L->Last = temp;
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

    if(L->Act == NULL || L->Act == L->Last)     // Kontrola aktivity
        return;
    
    tDLElemPtr temp = L->Act->rptr;     // Priradenie nasledujuceho prvku do temp
    if(temp == L->Last){                // Ak je mazany prvok posledny
        L->Last = L->Act;               // Aktivny sa stava poslednym
        L->Act->rptr = NULL;            // Ukonci zoznam
    }
    else{
        L->Act->rptr = temp->rptr;      // Inak nastavenie praveho ukazatela
        temp->rptr->lptr = L->Act;      // Spoj zoznam   
    }

    free(temp);
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	
/* Podobny princip ako DLPostDelete */

    if(L->Act == NULL || L->Act == L->First)
        return;
    
    tDLElemPtr temp = L->Act->lptr;
    if(temp == L->First){
        L->First = L->Act;
        L->Act->lptr = NULL;
    }
    else{
        L->Act->lptr = temp->lptr;
        temp->lptr->rptr = L->Act;
    }

    free(temp);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL)      // Kontrola aktivity
        return;

    tDLElemPtr new;

    if((new = malloc(sizeof(struct tDLElem))) == NULL){     // Kontrola alokacie
        DLError();
        return;
    }

	new->data = val;
    new->lptr = L->Act;             // Do predchadzajuceho pointeru prirad aktivny prvok
    new->rptr = L->Act->rptr;       // Do nasledujuceho pointeru prirad nasledujuci prvok aktivneho
    L->Act->rptr = new;             // Nasledujuci prvok aktivneho je teraz novy prvok
    if(L->Act != L->Last)           // Ak nie je aktivny prvok posledny
        new->rptr->lptr = new;       // Spoj zoznam aj druhou stranou
    else
        L->Last = new;              // Inak nastav pridany prvok ako posledny

}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

/* Podobny princip ako pri DLPostInsert */

	if(L->Act == NULL)
        return;

    tDLElemPtr new;

    if((new = malloc(sizeof(struct tDLElem))) == NULL){
        DLError();
        return;
    }

    new->data = val;
    new->rptr = L->Act;
    new->lptr = L->Act->lptr;
    L->Act->lptr = new;
    if(L->Act != L->First)
        new->lptr->rptr = new;
    else
        L->First = new;	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
    if(L->Act == NULL){     // Kontrola aktivity 
        DLError();
        return;
    }
	
    *val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
    if(L->Act == NULL)      // Kontrola aktivity
        return;

    L->Act->data = val;
    
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
    if(L->Act == NULL)      // Kontrola aktivity
        return;

    L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
    if(L->Act == NULL)      // Kontrola aktivity
        return;

    L->Act = L->Act->lptr;
	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
    return (L->Act != NULL);    // Ak aktivny nie je NULL == return nenulovu hodnotu
}

/* Konec c206.c*/
