/*
 * Subor: ppm.h
 * Projekt: IJC DU1, priklad b)
 * Datum: 20.3.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Funkcie pre citanie a zapisovanie obrazku vo formate ppm
 */

#include "error.h"

#ifndef PPM_H
#define PPM_H

#define RES_LIMIT 1000*1000*3

struct ppm{
    unsigned xsize;
    unsigned ysize;
    char data[];
};

struct ppm *ppm_read(const char *filename);


int ppm_write(struct ppm *p, const char *filename);


#endif
