/*
 * Subor: error.c
 * Projekt: IJC DU1, priklad b)
 * Datum: 20.3.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Vlastne funkcie na vypis chyb
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "error.h"

#ifndef MSG_PRINT
#define MSG_PRINT

void warning_msg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    va_end(ap);
}

void error_exit(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    va_end(ap);
    exit(1);   
}

#endif
