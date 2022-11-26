#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int cnt =0;
static int semid;

void p(){

}
int main(int argc, char *argv[]){
    pthread_t thread1;
    pthread_t thread2;

    union semun{
        int     val;
        struct  semid_ds *buf;
        unsigned short int *array;
    }arg;
    
}
