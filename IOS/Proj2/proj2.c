#include "proj2.h"

#define UNLOCKED 1
#define LOCKED 0

int *sharedA = NULL; // Premenna A
int sharedA_ID = 0;
int *sharedEND = NULL; // Pocet riderov ktory cestovali
int sharedEND_ID = 0;
int *sharedWAITING = NULL; // Pocet riderov ktory cakaju na zastavke
int sharedWAITING_ID = 0;

sem_t* sem_mutex; // mutex aby nenastupili ridery ktory nie su na zastavke
sem_t* sem_bus; // Semafor na nastupovanie riderov
sem_t* sem_aboard; // Semafor na BUS depart
sem_t* sem_multiplex; // Max pocet riderov na zastavke
sem_t* sem_end; // Semafor na ukoncovanie riderov
sem_t* sem_print; // Prednostne pravo pri vypisovani a zvacsovani premennej
    
FILE* file;

unsigned random_num(unsigned max){
    if (max == 0)
        return 0;
    srand((unsigned) time(NULL));
    return rand() % (max + 1);
}


void set_resources(int C){
    bool clean = false;
    
    if((sem_mutex = sem_open("/xhorna14_mutex", O_CREAT | O_EXCL, 0666, UNLOCKED)) == SEM_FAILED)
        clean = true;
    
    if((sem_bus = sem_open("/xhorna14_bus", O_CREAT | O_EXCL, 0666, LOCKED)) == SEM_FAILED)
        clean = true;
    
    if((sem_aboard = sem_open("/xhorna14_aboard", O_CREAT | O_EXCL, 0666, LOCKED)) == SEM_FAILED)
        clean = true;
    
    if((sem_multiplex = sem_open("/xhorna14_multiplex", O_CREAT | O_EXCL, 0666, C)) == SEM_FAILED)
        clean = true;
    
    if((sem_end = sem_open("/xhorna14_end", O_CREAT | O_EXCL, 0666, LOCKED)) == SEM_FAILED)
        clean = true;

    if((sem_print = sem_open("/xhorna14_print", O_CREAT | O_EXCL, 0666, UNLOCKED)) == SEM_FAILED)
        clean = true;


    if((sharedA_ID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1)
        clean = true;

    if((sharedA = (int*) shmat(sharedA_ID, NULL, 0)) == NULL)
        clean = true;
    
    if((sharedEND_ID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1)
        clean = true;

    if((sharedEND = (int*) shmat(sharedEND_ID, NULL, 0)) == NULL)
        clean = true;
    
    if((sharedWAITING_ID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1)
        clean = true;

    if((sharedWAITING = (int*) shmat(sharedWAITING_ID, NULL, 0)) == NULL)
        clean = true;
    
    if(clean)
        clean_resources();

    *sharedA = 1;
    *sharedEND = 0;
    *sharedWAITING = 0;
}

void clean_resources(){

    shmctl(sharedA_ID, IPC_RMID, NULL);
    shmctl(sharedEND_ID, IPC_RMID, NULL);
    shmctl(sharedWAITING_ID, IPC_RMID, NULL);

    sem_close(sem_mutex);
    sem_close(sem_bus);
    sem_close(sem_aboard);
    sem_close(sem_multiplex);
    sem_close(sem_end);
    sem_close(sem_print);

    sem_unlink("/xhorna14_mutex");
    sem_unlink("/xhorna14_bus");
    sem_unlink("/xhorna14_aboard");
    sem_unlink("/xhorna14_multiplex");
    sem_unlink("/xhorna14_end");
    sem_unlink("/xhorna14_print");

    if(fclose(file) != 0){
        fprintf(stderr, "ERROR: File could not be closed\n");
        exit(1);
    }
}

args_t parse_args(int argc, char* argv[]){
    
    args_t args;

    if(argc != 5){
        fprintf(stderr, "ERROR: Missing arguments\n");
        exit(1);    
    }

    // Kontrola ci su vsekty argumenty cisla
    char* temp;
    int n;
    n = strtol(argv[1], &temp, 10);
    if(temp[0] == '\0' && n > 0)
        args.R = n;
    else{
        fprintf(stderr, "ERROR: Wrong arguments\n");
        exit(1);
    }

    n = strtol(argv[2], &temp, 10);
    if(temp[0] == '\0' && n > 0)
        args.C = n;
    else{
        fprintf(stderr, "ERROR: Wrong arguments\n");
        exit(1);
    }
    
    n = strtol(argv[3], &temp, 10);
    if(temp[0] == '\0' && n >= 0 && n <= 1000)
        args.ART = n;
    else{
        fprintf(stderr, "ERROR: Wrong arguments\n");
        exit(1);
    }

    n = strtol(argv[4], &temp, 10);
    if(temp[0] == '\0' && n >= 0 && n <= 1000)
        args.ABT = n;
    else{
        fprintf(stderr, "ERROR: Wrong arguments\n");
        exit(1);
    }
    
    return args;
}

void generate_riders(args_t args){
    for(int i = 0; i < args.R; i++){
        usleep(random_num(args.ART) * 1000); // Cas generovania ridera
        pid_t RID = fork();
        if(RID < 0){          
            fprintf(stderr, "ERROR: Fork failure\n");
            clean_resources();
            exit(1);
        }
        else if(RID == 0){ // Child rider
            rider(i+1); 
            exit(0);
        }
     }
    for(int i = 0; i < args.R; i++) // Cakaj na skoncenie riderov
        wait(NULL);
}

void rider(int ID){
    sem_wait(sem_print);
    fprintf(file, "%d\t: RID %d\t: start\n", (*sharedA)++, ID);
    sem_post(sem_print);

    sem_wait(sem_multiplex); //Vstup na zastavku ak je tam miesto
    sem_wait(sem_mutex); //MUTEX
    (*sharedWAITING)++; // Zvacsi cakajucich na zastavke

    sem_wait(sem_print);
    fprintf(file, "%d\t: RID %d\t: enter: %d\n", (*sharedA)++, ID, (*sharedWAITING));
    sem_post(sem_print);

    sem_post(sem_mutex); //MUTEX

    sem_wait(sem_bus); // Cakaj na nastupenie
    sem_post(sem_multiplex); //Vystup zo zastavky

    sem_wait(sem_print);
    fprintf(file, "%d\t: RID %d\t: boarding\n", (*sharedA)++, ID); // Nastup
    (*sharedEND)++; // Zvacsi pocet riderov ktory nastupil
    (*sharedWAITING)--; // Zmensi pocet riderov ktory cakaju
    sem_post(sem_print);

    if(*sharedWAITING == 0) // Ak nikto necaka
        sem_post(sem_aboard); // Daj zelenu autobusu na depart
    else
        sem_post(sem_bus); // Inak dalsi rider nastupuje
    
    sem_wait(sem_end); // Pockaj na povolenie finishovat
    sem_post(sem_end); // Otvor finish pre dalsieho

    sem_wait(sem_print);
    fprintf(file, "%d\t: RID %d\t: finish\n", (*sharedA)++, ID);
    sem_post(sem_print);
}

void bus(int ABT){
    sem_wait(sem_mutex); //MUTEX
    
    sem_wait(sem_print);
    fprintf(file, "%d\t: BUS\t: arrival\n", (*sharedA)++);
    sem_post(sem_print);

    if(*sharedWAITING > 0){ // Ak niekto caka na zastavke
        sem_wait(sem_print);
        fprintf(file, "%d\t: BUS\t: start boarding: %d\n", (*sharedA)++, (*sharedWAITING));
        sem_post(sem_print);
        
        while(!sem_trywait(sem_end)); // Zatvor moznost finishnut

        sem_post(sem_bus); // Otvor nastupovanie

        sem_wait(sem_aboard); // Cakaj na nastupenie vsetkych

        sem_wait(sem_print);
        fprintf(file, "%d\t: BUS\t: end boarding: %d\n", (*sharedA)++, (*sharedWAITING));
        sem_post(sem_print);
        }

    sem_wait(sem_print);
    fprintf(file, "%d\t: BUS\t: depart\n", (*sharedA)++);
    sem_post(sem_print);
    
    sem_post(sem_mutex); //MUTEX

    usleep(random_num(ABT) * 1000); // Cestuj

    sem_wait(sem_print);
    fprintf(file, "%d\t: BUS\t: end\n", (*sharedA)++);
    sem_post(sem_print);
    sem_post(sem_end); // Otvor finishovanie riderov
}

int main(int argc, char* argv[])
{
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    
    args_t args = parse_args(argc, argv);

    file = fopen("proj2.out", "w+");
        if(file == NULL){
            fprintf(stderr, "ERROR: File could not be opened\n");
            return 1;
        }

    setbuf(file, NULL);
    set_resources(args.C);

    pid_t BUS = fork();
    if(BUS < 0){
        fprintf(stderr, "ERROR: Fork failure\n");
        clean_resources();
        return 1;
    }

    //Child BUS proces
    else if(BUS == 0){
        sem_wait(sem_print);
        fprintf(file, "%d\t: BUS\t: start\n", (*sharedA)++);
        sem_post(sem_print);
        
        //Cestuj az pokial necestovali vsetci jazdci
        while(*sharedEND != args.R){
            bus(args.ABT);
        }

        sem_wait(sem_print);
        fprintf(file, "%d\t: BUS\t: finish\n", (*sharedA)++);
        sem_post(sem_print);
        exit(0);
    }
    else{
        pid_t RID_GEN = fork();
        if(RID_GEN < 0){
            fprintf(stderr, "ERROR: Fork failure\n");
            clean_resources();
            return 1;
        }
        //Child rider generator
        else if(RID_GEN == 0){
            generate_riders(args);
            exit(0);
            }
        }
    // Cakaj na koniec autobusu a generatoru
    wait(NULL);
    wait(NULL);
    clean_resources();
    return 0;
}
