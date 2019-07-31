#include "header/instrlist.h"

/* Vypíše chybu pri nesprávnej práci so zoznamom */
void DLError() {
    printf ("*ERROR* The program has performed an illegal operation.\n");
    return;
}

void DLInitList (tDLList *L) {
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
// Posuva First a cez Act uvolnuje paměť
    while(L->First!= NULL){
        L->Act = L->First;
        L->First = L->First->rptr;
        gb_free(L->Act->instruction);
        L->Act->instruction = NULL;
        gb_free(L->Act);
    }

// Nastaví ukazatele na stav po inicializácii
    L->Act = NULL;
    L->Last = NULL;
}

void DLPostInsertList (tDLList *L, tDLList *M)
{
    //Ak sú listy neaktívne alebo L neni akt. volá DLError
    if(!L || !M || !L->Act ){
        DLError();
        return;
    }
    //Ak je L->Act posledný nastaví L->Last na M->Last
    if(L->Act == L->Last){
        L->Last = M->Last;
        M->First->lptr = L->Act;
        L->Act->rptr = M->First;
    }
    else{
        M->Last->rptr = L->Act->rptr;
        L->Act->rptr->lptr = M->Last;
        L->Act->rptr = M->First;
        M->First->lptr = L->Act;
    }
}

void DLPreInsertList (tDLList *L, tDLList *M)
{
    //Ak sú listy neaktívne alebo L neni akt. volá DLError
    if(!L || !M || !L->Act ){
        DLError();
        return;
    }
    //Ak je L->Act prvý nastaví L->First na M->First
    if(L->Act == L->First){
        L->First = M->First;
        L->Act->lptr = M->Last;
        M->Last->rptr = L->Act;
    }
    else{
        M->First->lptr = L->Act->lptr;
        M->Last->rptr = L->Act;
        L->Act->lptr->rptr = M->First;
        L->Act->lptr = M->Last;
    }
}


void DLInsertFirst (tDLList *L, char *instruction) {
  // Naalokujem prvok a nastavim hodnoty
  tDLElemPtr New = gb_malloc(sizeof(struct tDLElem));
  // Pri chybe alokácie ukončí program s error kódom 99
  if(New == NULL){
      gb_exit_process(99);
      return;
  }

  New->instruction = gb_malloc(sizeof(char)*(strlen(instruction)+1));
  if(!New->instruction){
      gb_exit_process(99);
      return;
  }

  strcpy(New->instruction,instruction);
  New->lptr = NULL;
  New->rptr = L->First;

  //Ak je zoznam prazdny nastavi aj ukazovatel Last na novy prvok
  if(L->First == NULL)
      L->Last = New;
  else
      L->First->lptr = New;
  L->First = New;
}

void DLInsertLast(tDLList *L, char *instruction) {
  //Naalokujem novy prcok a nastavim hodnoty
  tDLElemPtr New = gb_malloc(sizeof(struct tDLElem));
  // Pri chybe alokácie ukončí program s error kódom 99
  if(New == NULL){
      gb_exit_process(99);
      return;
  }

  New->instruction = gb_malloc(sizeof(char)*(strlen(instruction)+1));
  if(!New->instruction){
      gb_exit_process(99);
      return;
  }

  strcpy(New->instruction,instruction);
  New->rptr = NULL;
  New->lptr = L->Last;

  //Ak je zoznam prazdny nastavi novy prvok ako First
  if(L->First == NULL)
      L->First = New;
  else
      L->Last->rptr = New;
  L->Last = New;
}

void DLFirst (tDLList *L) {
  L->Act = L->First;
}

void DLLast (tDLList *L) {
  L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, char **val) {
  if(L->First == NULL){
      DLError();
      return;
  }
  else{
      *val = gb_malloc(sizeof(char)*(strlen(L->First->instruction)+1));
      if(*val == NULL)
          gb_exit_process(99);
      strcpy(*val,L->First->instruction);
  }

}

void DLCopyLast (tDLList *L, char **val) {
  if(L->First == NULL){
      DLError();
      return;
  }
  else{
      *val = gb_malloc(sizeof(char)*(strlen(L->Last->instruction)+1));
      if(*val == NULL)
          gb_exit_process(99);
      strcpy(*val,L->Last->instruction);
  }
}

void DLDeleteFirst (tDLList *L) {
  //Ak je zoznam prazdny nic sa nedeje
  if(L->First == NULL)
      return;

  //Ak je aktivny prvy prvok aktivita sa straca
  if(L->First == L->Act)
      L->Act = NULL;

  //Pomocny ukazatel na mazany prvok
  tDLElemPtr Pom = L->First;

  //Ak je prvy prvok jediny, zrusi sa zoznam
  if(L->First->rptr == NULL){
    L->First = NULL;
    L->Last = NULL;
  }
  else{
  L->First = L->First->rptr;
  L->First->lptr = NULL;
  }

  //Uvolni prvy prvok
  gb_free(Pom->instruction);
  gb_free(Pom);
}

void DLDeleteLast (tDLList *L) {
  //Ak je zoznam prazdny nic sa nedeje
  if(L->First == NULL)
      return;

  //Ak je aktivny posledny prvok aktivita sa straca
  if(L->Last == L->Act)
      L->Act = NULL;

  //Pomocny ukazatel na mazany prvok
  tDLElemPtr Pom = L->Last;

  //Ak je posledny prvok jediny, zrusi sa zoznam
  if(L->Last->lptr == NULL){
      L->First = NULL;
      L->Last = NULL;
  }
  else{
      L->Last = L->Last->lptr;
      L->Last->rptr = NULL;
  }
    //Uvolni posledny prvok
    gb_free(Pom->instruction);
    gb_free(Pom);
}

void DLPostDelete (tDLList *L) {
  //Ak nie je Act alebo je Act Last nič nerobí
  if(L->Act == NULL || L->Last == L->Act)
      return;

  //Pomocny ukazatel na mazany prvok
  tDLElemPtr Pom = L->Act->rptr;

  //Ak je mazany prvok posledny nastavi Last na Act
  if(L->Last == Pom){
      L->Last = L->Act;
      L->Act->rptr = NULL;
  }
  else{  //Ak nie je nakonci nastavi spravne pointre
      L->Act->rptr = Pom->rptr;
      Pom->rptr->lptr = L->Act;
  }

  //Uvolni mazany prvok
  gb_free(Pom->instruction);
  gb_free(Pom);
}

void DLPreDelete (tDLList *L) {
  //Ak nie je Act alebo je Act Last nič nerobí
  if(L->Act == NULL || L->First == L-> Act)
      return;

  //Pomocny ukazatel na mazany prvok
  tDLElemPtr Pom = L->Act->lptr;

  //Ak je mazany prvok prvy nastavy First na Act
  if(L->First == Pom){
      L->First = L->Act;
      L->Act->lptr = NULL;
  }
  else{  //Ak nie je nakonci nastavi spravne pointre
      L->Act->lptr = Pom->lptr;
      Pom->lptr->rptr = L->Act;
  }
  //Uvolni prvok
  gb_free(Pom->instruction);
  gb_free(Pom);
}

void DLPostInsert (tDLList *L,  char *instruction) {
  if(L->Act == NULL)
      return;

  //Vytvorenie noveho prvku
  tDLElemPtr New = gb_malloc(sizeof(struct tDLElem));
  if(New == NULL){
       gb_exit_process(99);
      return;
  }
  //Inicializujem data noveho prvku
  New->instruction = gb_malloc(sizeof(char)*(strlen(instruction)+1));
  if(!New->instruction){
       gb_exit_process(99);
      return;
  }

  strcpy(New->instruction,instruction);
  New->lptr = L->Act;

  //Ak je Act posledny nastavy Last na Novy prvok a nastavi pointre
  if(L->Act == L->Last){
      L->Last = New;
      New->rptr = NULL;
  }
  else{
      New->rptr = L->Act->rptr;
      L->Act->rptr->lptr = New;
  }
    L->Act->rptr = New;
}

void DLPreInsert (tDLList *L, char *instruction) {
  if(L->Act == NULL)
      return;
  //Vytvorenie noveho prvku
  tDLElemPtr New = gb_malloc(sizeof(struct tDLElem));
  if(New == NULL){
       gb_exit_process(99);
      return;
  }
  //Inicializujem data noveho prvku
  New->instruction = gb_malloc(sizeof(char)*(strlen(instruction)+1));
  if(!New->instruction){
       gb_exit_process(99);
      return;
  }

  strcpy(New->instruction,instruction);
  New->rptr = L->Act;

  //Ak je Act prvy nastavy First na Novy prvok a nastavi pointre
  if(L->Act == L->First){
      L->First = New;
      L->Act->lptr = New;
  }
  else{
      New->lptr = L->Act->lptr;
      L->Act->lptr->rptr = New;
  }
    L->Act->lptr = New;
}

void DLCopy (tDLList *L, char **val) {
  if(L->Act != NULL){
      *val = gb_malloc(sizeof(char)*(strlen(L->Act->instruction)+1));
      if(*val == NULL)
          gb_exit_process(99);
      strcpy(*val,L->Act->instruction);
  }
  else{
      DLError();
      return;
  }
}

void DLActualize (tDLList *L, char  *val) {
  if(L->Act != NULL){
    L->Act->instruction = realloc(L->Act->instruction,(sizeof(char)*(strlen(val)+1)));
    strcpy(L->Act->instruction,val);
  }
}

void DLSucc (tDLList *L) {
  if(L->Act != NULL){
    L->Act = L->Act->rptr;
  }
}


void DLPred (tDLList *L) {
  if(L->Act != NULL){
    L->Act = L->Act->lptr;
  }
}

int DLActive (tDLList *L) {
  return L->Act == NULL ? 0 : 1;
}

void DLPrintList (tDLList *L){
    // Pomocny prvok pre prechazanie zoznamom
    tDLElemPtr TMP;
    TMP = L->First;
    while(TMP){
        printf("%s",TMP->instruction);
        TMP = TMP->rptr;
    }
}
