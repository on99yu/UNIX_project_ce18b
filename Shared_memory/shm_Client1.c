#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>

#define KEY_NUM 60063


// 끝말잇기 구조체
struct shiritori {
    char Recentword[50];
    char Lastword[50];
    int flag, server_pid, client1_pid, client2_pid;
};

// 구조체 참조 포인터 생성
struct shiritori* shm_ptr;

// 쓰레드 선언
pthread_t Input, Last;
pthread_mutex_t mutex;
void FromServer(int signum);
void *InputWord();
void *Lastword();
int main()
{
    // 변수 선언
    int pid = getpid();
    int shm_id;
    pthread_mutex_init(&mutex, NULL);
    signal(SIGUSR1,FromServer);

    //  공유 메모리 접근
    printf("========================================\n");
    printf("끝말잇기 프로그램 실행...\n");
    printf("========================================\n");
    if (-1 == (shm_id = shmget((key_t)KEY_NUM, sizeof(struct shiritori), IPC_CREAT | 0666))){
        printf("클라1 : ERROR - 공유 메모리 접근 실패\n");
        return -1;
    }else{
        printf("클라1 : 공유 메모리 접근 완료\n");
    }

    // 공유 메모리 첨부
    if ((void *)-1 == (shm_ptr = shmat(shm_id, (void *)0, 0))){
        printf("클라1 : ERROR - 공유 메모리 첨부 실패\n");
    }else{
        printf("클라1 : 공유 메모리 첨부 완료\n");
    }
    shm_ptr->client1_pid = pid;
    printf("========================================\n");
    printf("클라1 : 끝말잇기 준비완료\n");
    printf("========================================\n");
    printf("랜덤 제시어 :  %s\n", (char*)shm_ptr->Recentword);
    // 반복문 필요 구간
    while(shm_ptr->flag){
        pthread_create(&Last, NULL, *Lastword, NULL);
        pthread_join(Last,NULL);
        pthread_create(&Input, NULL, *InputWord, NULL);
        pthread_join(Input,NULL);
        if(-1 == kill(shm_ptr->server_pid,SIGUSR1)){
            printf("클라1 : ERROR - 시그널 전송 실패\n");
        }
        printf("========================================\n");
        printf("클라이언트2이(가) 입력중...\n");
        pause();

    }
    printf("끝말잇기가 종료되었습니다.\n");
    pthread_mutex_destroy(&mutex);
    if(-1 == shmdt((void*)shm_ptr)){
        printf("클라1 : ERROR - 공유 메모리 해제 실패\n");
    }else{
        printf("클라1 : 공유 메모리 해제 완료");
    }
    return 0;
}

void FromServer(int signum){
    if(signum == SIGUSR1){
        return (void)0;
    }
}

void *InputWord(){
    pthread_mutex_lock(&mutex);
    printf("단어 입력 : ");
    fgets(shm_ptr->Recentword, 50, stdin);
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void *Lastword(){
    pthread_mutex_lock(&mutex);
    printf("========================================\n");
    printf("상대방의 단어 : %s\n", (char*)shm_ptr->Lastword);
    pthread_mutex_unlock(&mutex);
    return NULL;
}