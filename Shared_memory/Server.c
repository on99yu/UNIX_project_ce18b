#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY_NUM 600601
#define MEM_SIZE 4096

int main()
{
    int shm_id;
    void *shm_addr;

    // 공유 메모리 생성
    if (-1 == (shm_id = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666)))
    {
        printf("서버 : ERROR - 공유 메모리 생성 실패\n");
        return -1;
    }
    else
    {
        printf("서버 : 공유 메모리 생성 완료\n");
        printf("서버 : 공유 메모리 ID - %d\n", shm_id);
    }
    // 공유 메모리 접근
    if ((void *)-1 == (shm_addr = shmat(shm_id, (void *)0, 0)))
    {
        printf("서버 : ERROR - 공유 메모리 첨부 실패\n");
    }
    else
    {
        printf("서버 : 공유 메모리 첨부 완료\n");
    }

    // if (-1 == shmdt(shm_addr))
    // {
    //     printf("ERROR - 공유 메모리 해제 실패\n");
    // }
    // else
    // {
    //     printf("공유 메모리 해제 완료\n");
    //     printf("공유 메모리 ID - %d\n", shm_id);
    // }
}