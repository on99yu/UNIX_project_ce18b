#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define FILLED 0
#define Ready 1
#define NotReady -1
#define KEY_NUM 600601

struct memory{
    char buff[100];
    int status, pid1, pid2;
};
struct memory* shmptr;
void handler(int signum);

int main(){
    int pid = getpid();

    int shmid;

    shmid = shmget(KEY_NUM, sizeof(struct memory), IPC_CREAT | 0666);

    // shmat을 통해 공유메모리 식별자가 반환된다.
    shmptr = (struct memory*)shmat(shmid, NULL,0);
    
    shmptr->pid1 = pid;
    shmptr->status = NotReady;

    signal(SIGUSR1, handler);
    
    while(1){
        while(shmptr->status != Ready)
            continue;
        sleep(1);

        printf("User1: ");
        fgets(shmptr->buff, 100, stdin);
         
        shmptr->status = FILLED;
        
        kill(shmptr->pid2, SIGUSR2);

    }
    shmdt((void*)shmptr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
void handler(int signum){

    // signum1이 SIGUSR1이라는 뜻은 user1이 메세지를 받았다는 뜻이다.
    if(signum == SIGUSR1){
        printf("Received User2:");
        puts(shmptr->buff);
    }
}