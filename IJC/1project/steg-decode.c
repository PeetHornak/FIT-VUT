/*
 * Subor: steg-decode.c
 * Projekt: IJC DU1, priklad b)
 * Datum: 20.3.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Dekodovanie obrazka a vypis tajnej spravy
 */

#include <stdio.h>
#include <limits.h>

#include "bit_array.h"
#include "eratosthenes.h"
#include "ppm.h"
#include "error.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
        error_exit("Spatny pocet argumentov");
    
    struct ppm *pic = ppm_read(argv[1]);

    if(pic == NULL){
        free(pic);
        return -1;
    }

    bit_array_create(bitarray, RES_LIMIT);
    eratosthenes(bitarray);
    
    unsigned int printchar = 0;
    unsigned int bitcount = 0;
    unsigned int powers[8] = {1, 2, 4, 8, 16 ,32, 64, 128};
    unsigned int isnull = 0;
    unsigned int i = 11;

    while(i < RES_LIMIT)
    {
        /* Ak je prvocislo, zapis LSB do printchar na spravne miesto */
        if(bit_array_getbit(bitarray,i) == 0)
        {
            printchar += powers[bitcount] * (pic->data[i] & 1);
            bitcount++;
            if(bitcount == CHAR_BIT)
            {
                printf("%c", printchar);
                if(printchar == '\0')
                {
                    printf("\n");
                    isnull = 1;
                    break;
                }
                bitcount = 0;
                printchar = 0;
            }
        }   
        i += 2;
    }

    if(isnull == 0){
        free(pic);
        error_exit("Text nie je zakonceny NUL characterom");
    }

    free(pic);
    
    return 0;
}
