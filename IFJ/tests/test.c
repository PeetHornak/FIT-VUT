/** test.c
 * This file implements testing interface.
 * Author: Nikolas Patrik xpatri00
 * Collaborators: 
 *      Peter Hornak   xhorna14
 *      Matej Jancek   xjance00
 *      Robert Hubinak xhubin03 
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "test.h"

unsigned ti_number_of_tests_called = 0;
unsigned ti_number_of_passed_tests = 0;

/**
 * Here are defined some macros to graphically differenciate test units.
 */
#define TI_TEST_BE fprintf(stderr,"%s\n","====================================================")
#define TI_EVAL_BE fprintf(stderr,"%s\n","----------------------------------------------------")
#define TI_PRINT_BE fprintf(stderr,"%s\n","****************************************************")

void ti_init_functor(TFunctor *t, Iter i, Printer p, Comparator c)
{
    t->iterator = i;
    t->printing = p;
    t->cmp      = c;
}

int ti_print_elements(void *begin, void *end, TFunctor t)
{
    int count=0;
    TI_TEST_BE;
    for(void *i=begin; i != end ; i = t.iterator(i))
    {
        TI_PRINT_BE;
        t.printing(i);
        count++;
        TI_PRINT_BE;
    }
    TI_TEST_BE;
       
    return count;
}

int ti_compare_output_expected(void *first, void* second, TFunctor t)
{
    int res = t.cmp(first, second);
    ti_number_of_tests_called++;
    if(res == 0)
        ti_number_of_passed_tests++;
    return res;
}

int ti_printcompare_elements(void *first1,void *end1,void *first2,void *end2,
        TFunctor t)
{
    int len1=0,len2=0;
    for(void* i = first1 ; i != end1; i = t.iterator(i))
        len1++;
    for(void* i = first2 ; i != end2; i = t.iterator(i))
        len2++;
    if(len1 != len2)
        return -1; // Cant compare dynamic structure of different size.

    TI_TEST_BE;

    for(void *i = first1, *j = first2; i != end1; i = t.iterator(i), 
            j=t.iterator(j)) {
        TI_PRINT_BE;
        fprintf(stderr,"REAL VALUE:\n");
        t.printing(i);
        fprintf(stderr,"EXPECTED VALUE:\n");
        t.printing(j);
        TI_PRINT_BE; 
        ti_compare_output_expected(i,j,t);
    }
    
    TI_TEST_BE;

    return 0;
}


int ti_evaluate_test()
{
    TI_EVAL_BE;
    fprintf(stderr,"Testing results: %u of %u tests have passed.\n",
            ti_number_of_passed_tests, ti_number_of_tests_called);
    TI_EVAL_BE;
    return ti_number_of_passed_tests == ti_number_of_tests_called;
}

