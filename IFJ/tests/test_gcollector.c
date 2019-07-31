#include <stdio.h>
#include <stdlib.h>

#include "../src/header/garbagecollector.h"

#define randomsize 8

int main()
{
    garbage_collector_init();

    void *p1,*p2,*p3,*p4;

    puts("Alokujem pointre p1,p2,p3,p4");
    p1 = gb_malloc(randomsize);
    p2 = gb_malloc(randomsize);
    p3 = gb_malloc(randomsize);
    p4 = gb_malloc(randomsize);

    puts("Freeujem p2 a p4");
    gb_free(p2);
    gb_free(p4);

    puts("Znovu allokujem p2 a p4");
    p2 = gb_malloc(randomsize);
    p4 = gb_malloc(randomsize);

    puts("reallocujem p1");
    p1 = gb_realloc(p1, randomsize+randomsize);

    puts("registrujem pointer z originalneho mallocu");
    void *p5 = malloc(randomsize);
    gb_register_pointer(p5);

    puts("uvolnujem zdroje");
    gb_release_resources();

    puts("uvolnujem neexistujuci pointer p3");
    gb_free(p3);

    puts("rusim zdroje a exitujem kodom 5");
    gb_exit_process(5);
    return 0;
}
