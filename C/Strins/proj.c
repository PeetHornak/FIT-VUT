#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
//Insertne string INS, na poziciu Pos, a nahradi N znakov, novy Str moze mat
//maximalne Max Bytov
bool Edit(char *Str, int Max, unsigned Pos, unsigned N, char* Ins){
    int lench = strlen(Ins) - N;
    int lenstr = strlen(Str);
    if(lench + lenstr + 1 > Max)
        return false;

    if(lench >= 0){
        int n = Pos+N;
        for(int i = lenstr-1; i >= n; i--){
            Str[i+lench] = Str[i];
        }
    }
    else{
        int n = Pos+N;
        for(int i = n; i <= lenstr-1; i++){
            Str[i+lench] = Str[i];
        }
        for(int i = 0; i > lench ; i--){
            Str[strlen(Str)-1] = '\0';
        }
    }
    int j = 0;
    for(int i = Pos; i <= Pos + strlen(Ins) -1; i++){
        Str[i] = Ins[j];
        j++;
    }
    Str[lenstr] = '\0';
    return true;

}

int main(){
    char Str[10] = { "01234567" };
    char *Ins = { "ISE" };
    bool Vysledok = Edit(Str, 11, 1, 5, Ins);
    if (!Vysledok)
        fprintf(stderr,"ERROR: Str is too long\n");
    printf("%s\n", Str);
    return 0;

}
