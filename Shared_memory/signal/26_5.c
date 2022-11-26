#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 1024

void signalHandler(int signo);
int shmid;

int main(){
    void *shmaddr;

     /* 1234 키의 공유메모리 생성 */
    if((shmid=shmget((key_t)1234, SIZE, IPC_CREAT|0666)) == -1) {
       perror("shmid failed");
       exit(1);
    }

     /* shmid 공유메모리를 호출 프로세스 메모리 영역으로 첨부 */
    if((shmaddr=shmat(shmid, (void *)0, 0)) == (void *)-1) {
       perror("shmat failed");
       exit(1);
    }
     /* 공유메모리에 데이터 쓰기 */
    strcpy((char*)shmaddr,"Linux Programming");

    if(shmdt(shmaddr) == -1) {
       perror("shmdt failed");
       exit(1);
    }

    /* SIGINT 시그널 받으면 signalHandler 실행하도록 설정 */
    signal(SIGINT, signalHandler);
    pause();
}

void signalHandler(int signo)
{
    /* shmid 공유메모리 삭제 */
    if(shmctl(shmid, IPC_RMID, 0) == -1) {
       perror("shmctl failed");
       exit(1);
    }
    exit(0);
}