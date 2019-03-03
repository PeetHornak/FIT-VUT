#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define EOL 1

int ReadCSVItem(FILE *f, char delimiter, char *str, unsigned m){
    int c;
    int i = 0;
    while(delimiter != (c=fgetc(f))){
        if(c == EOF || c == '\n')
            break;
        if (i < (m-1)){
            str[i] = c;
            i++;
        }
    }
    str[i] = '\0';
    switch(c){
        case EOF:
            return EOF;
        case '\n':
            return EOL;
        case ';':
            return 0;
    }
    return 0;
}

int main(int argc, char *argv[]){
    
    int s1, s2;
    FILE *f;
    assert(argc > 2);
    assert(argc < 5);
    s1 = atoi(argv[1]);
    s2 = atoi(argv[2]);
    if(s1 > s2){
        int pom = s2;
        s2 = s1;
        s1 = pom;
    }
    if (argc == 3){
        f = stdin;
    }
    else{
        f = fopen(argv[3], "r");
        assert(f != NULL);
    }

    char pole[1024][1024];
    char temp[1024];
    int i = 0;
    int ret;
    while((ret = ReadCSVItem(f, ';', pole[i], 1024)) != EOF){
        if(ret == EOL){
            for(int j = 0; j <= i; j++){
                if(j == i)
                    printf("%s", pole[j]);
                else
                    printf("%s;", pole[j]);
            }
            printf("\n");
            i = 0;
            continue;
        }
        if(i+1 == s1){
            strcpy(temp, pole[i]);
        }
        else if(i+1 == s2){
            strcpy(pole[s1-1], pole[i]);
            strcpy(pole[i], temp);
        }
        i++;
    }
}
