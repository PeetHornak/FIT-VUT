/*
 * Subor: io.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Funkcia nacita prvych max -1 znakov az po prvy whitespace znake, vrati pocet nacitanych
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "io.h"

int get_word(char *s, int max, FILE *f){
    
    if(f == NULL || max <= 1)
        return EOF;

    char c;
    int i = 0;
    //Nacita vsetky biele znaky
    while(isspace(c = fgetc(f)))
        if(c == EOF)
            return EOF;
    //Nacitava po prvy biely znak
    while(!isspace(c)){
        i++;
        //Ak je nacitanych maximum prvkov
        if(i == max){
            while(!isspace(c = fgetc(f))){ //Nacitaj zbytok slova
                if(c == EOF){
                    return EOF;
                }
            }
            return i; 
        }
        else{
        s[i-1] = c; //Inak pridaj znak do stringu
        c = fgetc(f);
        }
    }
    s[i] = '\0'; //Pridaj koncovu nulu
    return i;
}
