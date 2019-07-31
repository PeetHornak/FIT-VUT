# Programming style
This document describes how C-programs are formatted in this project.

## Basic formatting
Every block of code should be indent by tab.
Tab equals for 4 space characters.

## Constants
Every constant should be written in capitals.

    #define CONSTANT 42
    enum constants{ HELLO, WORLD, AND, PEOPLE=99};

> Exception when constant is defined as const variable.

    const int basic_variable_name = 42;

## Variable, function names and typedef and struct definitions.
Variable and function name will use this notation:

    char this_such_a_long_variable_name;
    int foo_is_usually_used_as_function_name();

Structs use the same notation as normal variables except typedefs.
    
    typedef struct{
        int poo;
        char voo;
    }TContainer;

> Every typedef starts with T concatened with typedef name starting with
> **CAPITAL** letter. See **TContainer**.

## Functions
Every function should be nicely laid out like this:
    
    int foo()
    {
        /*block of code */
    }

Not like this:
    
    int poo(){
        /* NO!!! */
    }
    
## IF-clause, While, Do-While, For clauses
if-clause:

    if(True){
        /* block of code*/
    }

While and for clauses are the same as if clause.
Do-while pretty much same:
    
    do{
        /* block of code */
    }while(True);

## Switch formatting
    
    switch(True){
        case 42:
            /*block of code starts here*/
            break;
        default:
            /*same here*/
    }
## Lenght of the row
Every row in file should be shorter than 80 characters.
If you can't fit your expression into 80 chars use this dividing.

    /* This is long function call */
    get_me_food(my_cuisine, "Rice", main_meal, grams, price, long_variable,
                and_other_variable_names);
    /*or expression */
    int my_mood_level = (number_of_people_making_me_angry * number_of_things) /
                        (depression_level * project_number_for_this_week);

## Header files
Every header file must contain(example is header.h):

    #ifndef _HEADER_H_ 
    #define _HEADER_H_
    /* code_block */
    #endif


