/*
 * Subor: primes.c
 * Projekt: IJC DU1, priklad a)
 * Datum: 20.3.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Vypis poslednych 10 prvocisiel z danej mnoziny
 */

#include <stdio.h>

#include "eratosthenes.h"
#include "bit_array.h"

int main()
{
    bit_array_create(array, 222000000);
    eratosthenes(array);
    
    unsigned long velkost = bit_array_size(array) - 1;
    unsigned long n = 1;

    /* Vratenie sa v poli o 10 prvocisiel */ 
    for(; velkost > 1 && n < 10; velkost--){
        int hodnota = bit_array_getbit(array, velkost);
        if(!hodnota){
            n++;
        }
    }
    
    /* Vypis poslednych 10 prvocisiel */
    while(n > 0){
        if(bit_array_getbit(array, velkost) == 0){
            printf("%lu\n", velkost);
            n--;
        }
        velkost++;
    }  
    return 0;
}
