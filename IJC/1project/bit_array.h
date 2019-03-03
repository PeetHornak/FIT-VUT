/*
 * Subor: bit_array.h
 * Projekt: IJC DU1, priklad a)
 * Datum: 20.3.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Makra a inline funkcie
 */

#ifndef BIT_ARRAY
#define BIT_ARRAY

#include "error.h"
#include <limits.h>
#include <stdio.h>

typedef unsigned long bit_array_t[];

#define ulong_bits (sizeof(unsigned long) * CHAR_BIT)

#define bit_array_create(jmeno_pole, velikost) unsigned long jmeno_pole[(velikost / ulong_bits) + ((velikost % ulong_bits) ? 1 : 0) + 1] = {velikost, 0,}

#ifndef USE_INLINE

#define bit_array_size(jmeno_pole) jmeno_pole[0]

/*Nastavi bit podla bitovej masky*/
#define bit_array_setbit(jmeno_pole, index, vyraz)\
    (index >= 0 && index <= jmeno_pole[0] ? \
        (vyraz ? (jmeno_pole[index/ulong_bits + 1] |= (1UL << index%ulong_bits)) : (jmeno_pole[index/ulong_bits + 1] &= ~(1UL << index%ulong_bits)))\
    :(error_exit("Index %lu nepatri do bola bitov", (long)index),1))

/*Vrati hodnotu bitu na indexe pomocou bitovej masky a AND*/
#define bit_array_getbit(jmeno_pole, index)\
    (index >= 0 && index <= jmeno_pole[0] ? \
        (jmeno_pole[index/ulong_bits + 1] >> (index%ulong_bits) & 1UL)\
    :(error_exit("Index %lu nepatri do bola bitov", (long)index),1))
    

#else

static inline unsigned long bit_array_size(bit_array_t jmeno_pole)
{
    return jmeno_pole[0];
}

static inline void bit_array_setbit(bit_array_t jmeno_pole, unsigned long index, int vyraz)
{
    if(index >= 0 && index <= jmeno_pole[0]){
        if(vyraz){
            jmeno_pole[index/ulong_bits + 1] |= (1UL << index%ulong_bits);   
        }
        else{
            jmeno_pole[index/ulong_bits +1] &= ~(1UL << index%ulong_bits);
        }
    }
    else
        error_exit("Index %lu nepatri do pola bitov", (long)index);
}

static inline unsigned long bit_array_getbit(bit_array_t jmeno_pole, unsigned long index)
{
    if(index >= 0 && index <= jmeno_pole[0]){
        return (jmeno_pole[index/ulong_bits + 1] >> index%ulong_bits) & 1UL;
    }
    else{
        error_exit("Index %lu nepatri do pola bitov", (long)index);
        return -1;
    }
}

#endif
#endif
