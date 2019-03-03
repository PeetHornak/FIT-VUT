#include <stdio.h>
#include <stdlib.h>
/*POROVNAVANIE DVOCH SUBOROV CI SU IDENTICKE*/
int main(int argc, char ** argv)
{
    if(argc <2)
        puts("EROROR malo argumentov");
    FILE *fr,*fl;
    fr = fopen(argv[1],"rb");
    fl = fopen(argv[2],"rb");

    if(!(fr || fl))
        return 42;

    char znak1,znak2;
    while(fread(&znak1,1,1,fr) ||fread(&znak2,1,1,fl))
    {
        if(znak1 != znak2){
            puts("Error : jalfdkjaskjdflj");
            return 52;
        }
    }
    return 0;
 

}
