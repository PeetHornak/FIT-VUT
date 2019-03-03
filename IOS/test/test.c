#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <semaphore.h>

#include <string.h>
#include <ctype.h>

#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>

#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>

sem_t *boarding, 
*boarded , 
*unboarding, 
*unboarded , 
*waiting, 
*writing , 
*conditionboarding, 
*conditionunboarding;


/*
----------------------------------------------------------------------------------------------
   R = pocet procesu rided 
   C = kapacita autobusu
   ART = maximalna doba(ms) pre generovanie noveho procesu RIDER  ART >= 0 && ART <= 1000
   ABT = maximalna doba(ms) po ktoru BUS simuluje jazdu ABT >= 0 && ABT <= 1000
----------------------------------------------------------------------------------------------
   A = poradove cislo provadene akce 
   CR = pocet riders momentalne pritomych na zastavke 
   I = interni identifikator daneho pre proces daneho typu 
----------------------------------------------------------------------------------------------
*/ 

/*
            ========./proj2 R C ART ABT =======
            argc == 5 
*/  

int check_if_num(char *str) ; 
void arguments_check(int argc, char *argv[]) ; 
void SET_shared_mem(); 
void CREATE_semaphores(); 
void clean_shared_mem() ; 


int ART, ABT, C, R ;
int *shared_memory ;
int retval ; 
int mem_id ; 

pid_t pid ; 
pid_t main_process ; 
pid_t side_process ;

int del_check ;  


int main(int argc, char *argv[]){ 
    
    del_check = 0 ; 

    printf("Hello world!\n") ; 
    printf("Number of arguments is %d\n", argc) ; 
    arguments_check(argc, argv) ; 
    SET_shared_mem() ; 
    CREATE_semaphores() ; 

    FILE *output ; 
    output = fopen ("proj2.out" , "w+") ; 
    setbuf(output, NULL) ; 

    //clean_shared_mem() ;

    fclose(output);
    clean_shared_mem() ;

    return 0 ;
}

/*===================================================================================================================================
    Kontrola datoveho typu argumentov (decimal digit)
    uspech = 0 
    chyba = 1 
*/

int check_if_num(char *str)
{
    int i = 0 ; 
    int dlzka = strlen(str) ;
    while ( i < dlzka ) 
    {
        if (!isdigit(*(str+i))) 
        {
            fprintf(stderr, "Zle zadane argumenty\n") ;
            exit(1) ;  
        }
        i++ ; 
    } 
    return 0 ; 
}

//===================================================================================================================================


/*===================================================================================================================================
    Kontrola medzí a spravnosti hodnot z prikazovej riadky

*/


void arguments_check(int argc, char*argv[])
{ 
    if (argc != 5 )
    { 
        printf("Pocet argumentov nesedi!\n")  ;
        exit(1)  ;
    }
    else 
    {
        for (int i = 1 ; i < argc ; i++)
        {
            if (check_if_num(argv[i]))
             {
                 fprintf(stderr,"Zle zadanie argumentov!\n") ; 
                 return ;
             } 
        }
    }

    R =   atoi(argv[1]); 
    C =   atoi(argv[2]); 
    ART = atoi(argv[3]); 
    ABT = atoi(argv[4]); 

    if (C < 0)
    {
        fprintf(stderr,"Zle zadanie argumentov!'n") ; 
        exit(1) ; 
    }
    else 
    {
        printf("Capacity > 0 ?    |CHECK|\n") ;
    }

    if (ART < 0 || ART > 1000 )
    {
        fprintf(stderr, "Zle zadanie argumentov!\n") ; 
        exit(1) ; 
    }
    else 
    {
        printf("0 <= ART > 1000?  |CHECK|\n") ; 
    }

    if (ABT < 0 || ABT > 1000)
    {
        fprintf(stderr,"Zle zadanie argumentov!\n") ; 
        exit(1);
    }
    else 
    {
        printf("0 <= ABT > 1000?  |CHECK|\n");
    }

}

//=============================================================================================================================


/* 
-------------------------------------------------------------SETTING SEMAPHORES------------------------------------------------
*/


void CREATE_semaphores() 
{
    if((boarding = sem_open("/xhorna13boarding", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        // handle error
        fprintf(stderr, "Semaphor error /sem.xhorna13boarding.\n");
        shared_memory[3] = 2;
        exit(2);
    }

    if((boarded = sem_open("/xhorna13boarded", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        // handle error
        fprintf(stderr, "Semaphor error /sem.xhorna13boarded.\n");

        sem_close(boarding);

        sem_unlink("/xhorna13boarding");

        shared_memory[3] = 2;
        exit(2);
    }


    if((unboarding = sem_open("/xhorna13unboarding", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        // handle error
        fprintf(stderr, "Semaphor error /sem.xorna13unboarding.\n");
        sem_close(boarding);
        sem_close(boarded);
        sem_unlink("/xhorna13boarded");
        sem_unlink("/xhorna13boarding");
        

        shared_memory[3] = 2;
        exit(2);
    }

    if((unboarded = sem_open("/xhorna13unboarded", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        // handle error
        fprintf(stderr, "Semaphor error /sem.xhorna13unboarded.\n");

        sem_close(boarding);
        sem_close(boarded);
        sem_close(unboarding);

        sem_unlink("/xhorna13boarding");
        sem_unlink("/xhorna13boarded");
        sem_unlink("/xhorna13unboarding");

        shared_memory[3] = 2;
        exit(2);
    }

    if((waiting= sem_open("/xhorna13waiting", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED ){
        // handle error
        fprintf(stderr, "Semaphor error /sem.xhorna13waiting.\n");

        sem_close(boarding);
        sem_close(boarded);
        sem_close(unboarding);
        sem_close(unboarded);


        sem_unlink("/xhorna13boarding");
        sem_unlink("/xhorna13boarded");
        sem_unlink("/xhorna13unboarding");
        sem_unlink("/xhorna13unboarded");

        shared_memory[3] = 2;
        exit(2);
    }

    if((writing = sem_open("/xhorna13writing", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED ){
        // handle error
        fprintf(stderr, "Semaphor error /sem.xhorna13writing.\n");

        sem_close(boarding);
        sem_close(boarded);
        sem_close(unboarding);
        sem_close(unboarded);
        sem_close(waiting);

        sem_unlink("/xhorna13boarding");
        sem_unlink("/xhorna13boarded");
        sem_unlink("/xhorna13unboarding");
        sem_unlink("/xhorna13unboarded");
        sem_unlink("/xhorna13waiting");

        shared_memory[3] = 2;
        exit(2);
    }

    if((conditionboarding = sem_open("/xhorna13conditionboarding", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED ){
        // handle error
        fprintf(stderr, "Semaphor error /sem.xhorna13condtionboarding.\n");

        sem_close(boarding);
        sem_close(boarded);
        sem_close(unboarding);
        sem_close(unboarded);
        sem_close(waiting);
        sem_close(writing);

        sem_unlink("/xhorna13boarding");
        sem_unlink("/xhorna13boarded");
        sem_unlink("/xhorna13unboarding");
        sem_unlink("/xhorna13unboarded");
        sem_unlink("/xhorna13waiting");
        sem_unlink("/xhorna13writing");

        shared_memory[3] = 2;
        exit(2);
    }

    if( (conditionunboarding = sem_open("/xhorna13conditionunboarding", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED ){
        // handle error
        fprintf(stderr, "Semaphor error /sem.xhorna13condtionunboarding.\n");

        sem_close(boarding);
        sem_close(boarded);
        sem_close(unboarding);
        sem_close(unboarded);
        sem_close(waiting);
        sem_close(writing);
        sem_close(conditionboarding);

        sem_unlink("/xhorna13boarding");
        sem_unlink("/xhorna13boarded");
        sem_unlink("/xhorna13unboarding");
        sem_unlink("/xhorna13unboarded");
        sem_unlink("/xhorna13waiting");
        sem_unlink("/xhorna13writing");
        sem_unlink("/xhorna13conditionunboarding");

        shared_memory[3] = 2;
        exit(2);
    }
}

//------------------------------------------------------------------------------------------------------------------------------


/*
-----------------------------------------------------ENDING SEMAPHORES----------------------------------------------------------
*/

void free_semaphores(){

  sem_close(boarding);
  sem_close(boarded);
  sem_close(unboarding);
  sem_close(unboarded);
  sem_close(waiting);
  sem_close(writing);
  sem_close(conditionboarding);
  sem_close(conditionunboarding);


  sem_unlink("/xhorna13boarded");
  sem_unlink("/xhorna13writing");
  sem_unlink("/xhorna13unboarded");
  sem_unlink("/xhorna13boarding");
  sem_unlink("/xhorna13unboarding");
  sem_unlink("/xhorna13waiting");
  sem_unlink("/xhorna13conditionboarding");
  sem_unlink("/xhorna13conditionunboarding");

}

//------------------------------------------------------------------------------------------------------------------------------


void SET_shared_mem() 
{ 
    int shared_mem_size = sizeof(int)*4 ;
    mem_id = shmget(IPC_PRIVATE, shared_mem_size , IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR) ; 

    // kontrola inicializacie zdielanej pamäte
    if (mem_id < 0 )
    {
        fprintf(stderr,"==> Shared memory initialization ERROR  !\n") ; 
        exit(2) ; 
    }
    else
    {
        printf("==> Shared memory incitialized\n");
    }

    shared_memory = (int*) shmat (mem_id,0,0) ; 

    // kontrola priradenia/(attachement) zdielanej pamäte
    if (shared_memory == NULL)
    {
        fprintf(stderr, "==> Shared memory attachement failed!\n" );
        exit(2);
    }
    else 
    {
        printf("==> Shared memory attached\n");
    }

    //priradenie 
    shared_memory[0] = 1;
    shared_memory[1] = 1;
    shared_memory[2] = 1;
    shared_memory[3] = 0;
   
    printf("Shared memory[0] = %d\n", shared_memory[0]);
    printf("Shared memory[1] = %d\n", shared_memory[1]);
    printf("Shared memory[2] = %d\n", shared_memory[2]);
    printf("Shared memory[3] = %d\n", shared_memory[3]);
}

void clean_shared_mem()
{ 
    if (del_check)
    {
        exit(0) ; 
    }


    // detachement of shared memory 
    retval = shmdt(shared_memory) ; 

    if (retval == 0 )
    {
        printf("Memory DETACHED\n");
    }
    else 
    {
        if (retval < 0)
        {
            fprintf(stderr, "Detachement failure!\n" );
        }
    }

    // clearing the memory
    
    if ((shmctl(mem_id, IPC_RMID, NULL)) == 0 )
    {
        printf("Memory DELETED\n");
        del_check = 1 ; 
    }
    else
    {
        fprintf(stderr, "Memory wasnt't DELETED\n" );
    }
}

