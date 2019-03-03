#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h> 
#include <stdbool.h>
#include <unistd.h> 
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/shm.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/stat.h>
#include <sys/ipc.h>

typedef struct{
    int R;
    int C;
    int ART;
    int ABT;
} args_t;

unsigned random_num(unsigned max);

void set_resources(int R);

void clean_resources();

args_t parse_args(int argc, char* argv[]);

void generate_riders(args_t args);

void rider();

void bus(int ABT);
