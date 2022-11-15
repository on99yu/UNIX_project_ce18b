#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY_NUM 600601
#define MEM_SIZE 4096

int main()
{
    int shm_id;
    void *shm_addr;

    if (-1 == (shm_id = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666)))
    {
        printf("클라 : ERROR - 공유 메모리 접근 실패\n");
        return -1;
    }
    else
    {
        printf("클라 : 공유 메모리 접근 완료\n");
    }

    if ((void *)-1 == (shm_addr = shmat(shm_id, (void *)0, 0)))
    {
        printf("클라 : ERROR - 공유 메모리 첨부 실패\n");
    }
    else
    {
        printf("클라 : 공유 메모리 첨부 완료\n");
    }
}