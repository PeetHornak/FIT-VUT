#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>

#ifndef _STACK_H_
#define _STACK_H_

#define MAXSTACK 5000                     /* maximální počet prvků v zásobníku */

typedef struct my_stack{                          /* zásobník hodnot typu tBTNodePtr */
    int a[MAXSTACK];
    int top;
} tStack;

void SInit (tStack *S);

void SPush (tStack *S, int num);

int STopPop (tStack *S);

bool SEmpty (tStack *S);

int STop (tStack *S);

#endif
