#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define KEY_NUM 600601
#define MEM_SIZE 4096

typedef struct shiritori {
    char word[100];
    unsigned int id;
}SHIRITORI;

static int shm_create();
static int shm_dettach();
static int shm_attach();

int shm_id;
void *shm_addr;

int main()
{
    SHIRITORI *shm_info = NULL;
    

    printf("끝말잇기 프로그램 실행...");

    // 공유 메모리 생성
    if(shm_create() == 0){
        printf("끝말잇기 시작");
    }

    // 공유 메모리 접근
    int shm_attach();

    shm_info = (SHIRITORI*)shm_addr;
    
    while (1)
    {
        printf("%s\n", (char *)shm_addr);
        sleep(1);
    }

}
static int shm_create(){
    if (-1 == (shm_id = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666))){
        printf("서버 : ERROR - 공유 메모리 생성 실패\n");
        return -1;
    }else{
        printf("서버 : 공유 메모리 생성 완료\n");
        printf("서버 : 공유 메모리 ID - %d\n", shm_id);
        return 0;
    }
}
static int shm_attach(){
    if ((void *)-1 == (shm_addr = shmat(shm_id, (void *)0, 0))){
        printf("서버 : ERROR - 공유 메모리 첨부 실패\n");
        return -1;
    }else{
        printf("서버 : 공유 메모리 첨부 완료\n");
        return 0;
    }
}
static int shm_dettach(){
    if (-1 == shmdt(shm_addr))
    {
        printf("ERROR - 공유 메모리 해제 실패\n");
        return -1;
    }
    else
    {
        printf("공유 메모리 해제 완료\n");
        printf("공유 메모리 ID - %d\n", shm_id);
        return 0;
    }
}