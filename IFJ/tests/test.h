/** test.h
 * This header defines whole testing interface for unit testing.
 *
 * Testing interface define in this file is using 'ti' prefix,
 * to avoid name conflicts.
 *
 * Author: Nikolas Patrik xpatri00
 * Collaborators:
 *      Peter Hornak   xhorna14
 *      Matej Jancek   xjance00
 *      Robert Hubinak xhubin03
 *
 */ 
#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <stdlib.h>

extern unsigned ti_number_of_tests_called;
extern unsigned ti_number_of_passed_tests;

/**
 * Prints error message to stderr as perr_"type" with var as variable name,
 * val its current value and the eval which expected value for variable,
 */
#define perr_int(var,val,eval) fprintf(stderr,"%s:%d: %s = %d,expected %d.\n", \
        __FILE__,__LINE__, (var),(val),(eval))
#define perr_float(var,val,eval) fprintf(stderr,"%s:%d: %s = %f,expected %f.\n", \
        __FILE__,__LINE__, (var),(val),(eval))
#define perr_string(var,val,eval) fprintf(stderr,"%s:%d: %s = %s,expected %s.\n", \
        __FILE__,__LINE__, (var),(val),(eval))
#define perr_char(var,val,eval) fprintf(stderr,"%s:%d: %s = %c,expected %c.\n", \
        __FILE__,__LINE__, (var),(val),(eval))
#define perr_adress(var,val,eval) fprintf(stderr,"%s:%d: %s = %p,expected %p.\n", \
        __FILE__,__LINE__, (var),(void*)(val),(void*)(eval))

/*
 * Structure used for saving pointers to functions which are able to
 * work with dynamic data types.
 */
typedef void* (*Iter)(void*); //iterate through dynamic structure(DS)
typedef void (*Printer)(void*);//print element of DS
typedef int (*Comparator)(void*,void*);//compare elements of DS

typedef struct {
    Iter iterator;
    Printer printing;
    Comparator cmp;
}TFunctor;

/**
 * This function initialize functional structure which contains
 * pointer for function for using and working some of the dynamic structures.
 *
 * @brief Function handler for dynamic structures.
 *
 * @param t pointer for 'TFunctor' structure which handles working functions
 * @param i pointer to function which returns void pointer (void*) which is
 *          next element of dynamic data structure and take pointer to current
 * @param p pointer to function which prints element of dynamic data structure
 *          takes pointer to the element and returns nothing(void)
 * @param c pointer to function which compares to elements of dynamic data
 *          structure and returns int value similar to strcmp.
 */
void ti_init_functor(TFunctor *t, Iter i, Printer p, Comparator c);

/**
 * This function prints elements between 'begin' and 'end' pointers using cmp
 * function from 'TFunctor' structure.
 *
 * @param begin void pointer for begining of data structure
 * @param end void pointer for ending of data structure
 * @param t TFunctor struct which handles working functions for data structure
 * @return Number of elements which have been printed.
 */ 
int ti_print_elements(void* begin, void* end, TFunctor t);

/**
 * This function compares two elements of same data strucuture,
 * using function which returns 0 (zero) when both elements are equal, negative
 * number when first is less than second or positive number when others.
 * This function also increments global variable 'ti_number_of_tests_called'
 * and 'ti_number_of_passed_tests'.
 * 
 * @param first void pointer for element of data structure
 * @param second void pointer for element of data structure
 * @param t TFunctor struct which handles working functions for data structure
 * @return 0 when equal,-1 when first<second,1 whend first>second
 */
int ti_compare_output_expected(void *first, void *second, TFunctor t);

/**
 * This function is going through two same dynamic data structures and prints
 * them. It is similar to function above.
 *
 * @param first1 pointer to first element of first dynamic data structure(DDS)
 * @param end1 pointer to ending of first DDS
 * @param first2 pointer to first element of second DDS
 * @param end2 pointer to ending of second DDS
 * @param t TFunctor struct which handles working functions for data structure
 * @return -1 if the DDSs arent of same size otherwise returns 0
 */
int ti_printcompare_elements(void *first1,void *end1,void *first2,void *end2,
        TFunctor t);

/**
 * Prints how many of test have passed.
 * @return 1 if all test passed, otherwise 0
 */
int ti_evaluate_test();
    

#endif

/*
 * Example of use of test functions.
 *
void* next(void* current)
{
    return ((int*) current) + 1;
}

void arrayshow(void *current)
{
    printf("%d\n", *(int*) current);
}

int is_qual(void *f, void *s)
{
    return (*(int*) f == *(int*) s) ? 0:-1;
}

int main()
{
    TI_TEST_BE;
    int i=0;
    perr_int("s",i,0);
    
    perr_adress("i",&i,&i);

    int arr[5] = {1,2,3,4,5};
    int arr2[5]= {1,2,3,4,5};

    TFunctor t;
    ti_init_functor(&t, next, arrayshow, is_qual);
    ti_printcompare_elements(arr,&arr[5],arr2,&arr2[5],t);
    ti_evaluate_test();
}
*/

