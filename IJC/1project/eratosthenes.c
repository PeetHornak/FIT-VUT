/*
 * Subor: eratosthenes.c
 * Projekt: IJC DU1, priklad a)
 * Datum: 20.3.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Eratosthenovo sito
 *           Bit == 0 -> Prvocislo
 *           Bit == 1 -> Nie je prvocislo
 */

#include <stdio.h>

#include "bit_array.h"
#include "eratosthenes.h"

void eratosthenes(bit_array_t arr)
{
    /* Nastavi 0 a 1 na  1 */
    bit_array_setbit(arr, 0, 1);
    bit_array_setbit(arr, 1, 1);
    
    unsigned long velkost = arr[0];
    unsigned long limit = sqrt(velkost);

    /* Nastavi nasobky 2 na 1 */
    for(unsigned int j = 2; j*2 < velkost; j++){
        bit_array_setbit(arr, 2*j, 1);
    }
    
    /* Kontroluje kazde druhe cislo */
    for(unsigned int i = 2; i <= limit; i+=1){
        if(bit_array_getbit(arr, i) == 0)
            for(unsigned int j = 2; j*i < velkost; j++){
                bit_array_setbit(arr, i*j, 1);
            }
    }
}
