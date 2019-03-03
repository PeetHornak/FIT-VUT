/*
 * Subor: wordcount.c
 * Projekt: IJC DU2, priklad b)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Nacita slova zo stdin a vypise ich pocet vyskytov, vyuziva hashovaciu tabulku
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "io.h"
#include "htab.h"

#define HTAB_SIZE 42
#define WORD_LEN 10

void print_item(const char* key, unsigned* data){
    printf("%s\t%u\n", key, *data);
}

int main(){
    htab_t* htab = htab_init(HTAB_SIZE);

    if(htab == NULL){
        fprintf(stderr, "Table could not be initialized\n");
        return 1;
    }

    char word[WORD_LEN];
    int len;
    int too_long = 0;
    
    //Citanie po koniec vstupu
    while((len = get_word(word, WORD_LEN, stdin)) != EOF){
        if(strlen(word) == 0)
            continue;
        // Ak je slovo prilis dlhe vypis warning
        if(len == WORD_LEN && too_long == 0){
            fprintf(stderr, "WARNING: Some of the word were too long and had to be modified\n");
            too_long = 1;
        }
        //Prida slovo do tabulky
        if(htab_lookup_add(htab, word) == NULL){
            fprintf(stderr, "Word could not be added to table\n");
            htab_free(htab);
            return 1;
        }
    }
    //Vypise key a data
    htab_foreach(htab, print_item);
    htab_free(htab);
    return 0;
}
