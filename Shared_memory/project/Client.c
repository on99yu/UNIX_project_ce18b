#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY_NUM 600601
#define MEM_SIZE 4096

typedef struct shiritori {
    char word[100];
    unsigned int id;
}SHIRITORI;

int main()
{
    int shm_id;
    void *shm_addr;
    SHIRITORI *shm_info = NULL;

    //  공유 메모리 접근
    if (-1 == (shm_id = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666))){
        printf("클라 : ERROR - 공유 메모리 접근 실패\n");
        return -1;
    }else{
        printf("클라 : 공유 메모리 접근 완료\n");
    }

    // 공유 메모리 첨부
    if ((void *)-1 == (shm_addr = shmat(shm_id, (void *)0, 0))){
        printf("클라 : ERROR - 공유 메모리 첨부 실패\n");
    }else{
        printf("클라 : 공유 메모리 첨부 완료\n");
    }

    shm_info = (SHIRITORI*)shm_addr;

    char word[10];
    gets(word);
    while(1){
        sprintf((char *)shm_addr, "%d", word);
		sleep(1);
    }
}