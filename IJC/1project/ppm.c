/*
 * Subor: ppm.c
 * Projekt: IJC DU1, priklad b)
 * Datum: 20.3.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Funkcie pre citanie a zapisovanie obrazku vo formate ppm
 */

#include <limits.h>

#include "ppm.h"
#include "error.h"

struct ppm *ppm_read(const char *filename)
{
    unsigned int x, y;

    FILE *file = fopen(filename, "rb");

    if(file == NULL){
        warning_msg("Subor %s sa nepodarilo otvorit", filename);
        return NULL;
    }

    /* Nacitanie a skontrolovanie hlavicky suboru */
    if(fscanf(file, "P6 %u %u 255", &x, &y) != 2){
        if(fclose(file) == EOF)
            warning_msg("Subor %s sa nepodarilo zatvorit", filename);

        warning_msg("Spatny format suboru");
        return NULL;
    }

    if(x > 1000 || y > 1000){
        if(fclose(file) == EOF)
            warning_msg("Subor %s sa nepodarilo zatvorit", filename);
            
        warning_msg("Spatne vstupne data");
    }
    /* Skok na novy riadok, binarne data */
    fgetc(file);
    
    unsigned int data = x * y * 3;

    if(data > RES_LIMIT){
        if(fclose(file) == EOF)
            warning_msg("Subor %s sa nepodarilo zatvorit", filename);
        
        warning_msg("Maximalna velkost obrazovych dat prekrocila limit");
        return NULL;
    }

    /* Alokovanie pamaty pre strukturu ppm */
    struct ppm *pic = malloc(sizeof(struct ppm) + data * (sizeof(char)));
        if(pic == NULL){
            if(fclose(file) == EOF)
                warning_msg("Subor %s sa nepodarilo zatvorit", filename);
                
            warning_msg("Nepodarilo sa alokovat pamat");
            return NULL;
        }
    
    pic->xsize = x;
    pic->ysize = y;
    
    /* Nacitanie binarnyh dat z obrazku do struktury */
    if(fread(pic->data, sizeof(char), data, file) != data){
        if(fclose(file) == EOF)
            warning_msg("Subor %s sa nepodarilo zatvorit", filename);

        warning_msg("Problem pri nacitani dat zo suboru");
        free(pic);
        return NULL;
    }
    
    if(fclose(file) == EOF){
        warning_msg("Subor %s sa nepodarilo zatvorit", filename);
        return NULL;
    }
    return pic;
}

int ppm_write(struct ppm *p, const char *filename)
{
    unsigned int data = 3*p->xsize*p->ysize;

    FILE *file = fopen(filename, "wb");

    if(file == NULL){
        warning_msg("Subor %s sa nepodarilo otvorit", filename);  
        return -1;
    }
    
    /* Vpisanie hlavicky */
    fprintf(file, "P6\n%u %u\n255\n", p->xsize, p->ysize);

    /* Pisanie dat */
    if(fwrite(p->data, sizeof(char), data, file) != data){
        if(fclose(file) == EOF)
            warning_msg("Subor %s sa nepodarilo zatvorit", filename);
        
        warning_msg("Problem pri zapisovani dat do suboru");
        return -1;       
    }

    if(fclose(file) == EOF){
        warning_msg("Subor %s sa nepodarilo zatvorit", filename);
        return -1;
    }
    
    return 0;
}
