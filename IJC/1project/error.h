/*
 * Subor: error.h
 * Projekt: IJC DU1, priklad b)
 * Datum: 20.3.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Vlastne funkcie na vypis chyb
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ERROR_H
#define ERROR_H

void warning_msg(const char *fmt, ...);

void error_exit(const char *fmt, ...);

#endif
