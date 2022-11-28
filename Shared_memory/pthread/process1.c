#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>


typedef struct shm_struct_for_test
{
    pthread_mutex_t mtx;    // 공유메모리 동기화 용 뮤텍스
    int r;      // read index
    int data;   // data for test
}shm_t;   

shm_t *g_shm;

int main(int argc, char** argv){
    int shmid = 0;
    int shmkey =  0x12345678;
    pthread_mutexattr_t attr;

     if( (shmid = shmget(shmkey, (size_t)sizeof(shm_t), IPC_CREAT | 0666)) < 0 ) // shared memory 생성
    {
        printf("shmget error!\n");
        return -1;
    }
    printf("shmid=%d\n", shmid);

    if((g_shm=shmat(shmid,0,0))<0){
        printf("shmat error!\n");
        return -1;
    }

    if(pthread_mutexattr_init(&attr)){
        printf("init mutexattr error!\n");
    }

    if(pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED)){
        printf("setpshared mutexattr error!\n");
        return -1;
    }
}

