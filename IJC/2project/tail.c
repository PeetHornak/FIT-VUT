/*
 * Subor: tail.c
 * Projekt: IJC DU2, priklad a)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Zo suboru vypise poslednych -n riadkov o velkosti STR_LEN-2 znakov (+ '\n' a '\0')
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STR_LEN 1026

char *parse_args(int argc, char* argv[],int* n){
    if (argc < 1 || argc > 4){
        fprintf(stderr, "Missing arguments\n");
        exit(1);    
    }
    if(argc == 2){
        if (!strcmp(argv[1],"-n")){
            fprintf(stderr, "Missing arguments\n");
            exit(1);
        }
        else
            return argv[1];
    }
    if (argc == 3){
        if(!strcmp(argv[1], "-n")){
            char *temp;
            *n = strtol(argv[2], &temp, 10);
            if(*n >= 0 && temp[0] == '\0')
                return NULL;
            else{
                fprintf(stderr, "Wrong arguments\n");
                exit(1);
            }
        }
        else{
            fprintf(stderr, "Wrong arguments\n");
            exit(1);
        }
    }
    if (argc == 4){
        if(!strcmp(argv[1], "-n")){
            char *temp;
            *n = strtol(argv[2], &temp, 10);
            if(*n >= 0 && temp[0] == '\0')
                return argv[3];
            else{
                fprintf(stderr, "Wrong arguments\n");
                exit(1);
            }
        }
        else{
            fprintf(stderr, "Wrong arguments\n");
            exit(1);
        }
    }
    return NULL; //if argc == 1
}

int main(int argc, char* argv[]){
    FILE* file;
    int n = 10;
    char *filename = parse_args(argc, argv, &n);

    if(n < 1)
        return 0;
    
    char buff [n][STR_LEN];

    /* Otvorenie suboru */
    if(filename == NULL)
        file = stdin;
    else{
        file = fopen(filename, "r");
        if(file == NULL){
            fprintf(stderr, "File could not be opened\n");
            return 1;
        }
    }

    /* Nacitanie */
    int i = 0;
    int full = 0;
    int too_long = 0;
    //Nacitanie po koniec
    while(fgets(buff[i], STR_LEN-1, file) != NULL){
        //Ak je nacitanych maximum znakov a posledny znak nie je novy riadok
        if(strlen(buff[i]) == STR_LEN -2 && buff[i][STR_LEN-2] != '\n'){
            buff[i][STR_LEN-1] = '\n'; //pridaj znak noveho riadku
            if(!too_long)
                fprintf(stderr, "WARNING: Some of the words were too long and had to be modified\n");
            too_long = 1;
            while(fgetc(file) != '\n' || fgetc(file) != EOF) //Nacitaj zbytne znaky
                ;   
        }
        i++;
        if(i == n)
            full = 1;
        i = i % n;       
    }

    /* Vypis */
    if(full){
        for(int j = 0; j < n; j++){
            printf("%s", buff[i]);
            i++;
            i = i % n;
        }
    } 
    else
        for(int j = 0; j < i; j++)
            printf("%s", buff[j]);
    
    if(filename != NULL){
        if(fclose(file) != 0){
            fprintf(stderr, "File could not be closed\n");
            return 1;
        }
        else{
            return 0;
        }
    }
    return 0;
}
