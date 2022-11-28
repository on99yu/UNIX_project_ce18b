#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define KEY_NUM 60061

// 끝말잇기 구조체
struct shiritori {
    char word[50];
    char Lastword[50];
    int flag, server_pid, client1_pid, client2_pid;
};

// 구조체 참조 포인터 생성
struct shiritori* shm_ptr;

// 시작 제시어
char *Start_word[50] = {"얼룩말","안경집","마우스","콘센트","기차표"};

// 쓰레드 선언 
pthread_t start;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *MakeStartWord();
void FromClient1(int signum);
void FromClient2(int signum);

int main()
{   
    //변수 선언
    int pid = getpid();
    int shm_id;
    signal(SIGUSR1, FromClient1);
    signal(SIGUSR2, FromClient2);
    printf("========================================\n");
    printf("끝말잇기 프로그램 실행...\n");
    printf("========================================\n");

    // 공유메모리 생성
    if (-1 == (shm_id = shmget((key_t)KEY_NUM, sizeof(struct shiritori), IPC_CREAT | 0666))){
        printf("서버 : ERROR - 공유 메모리 생성 실패\n");
        exit(1);
    }else{
        printf("서버 : 공유 메모리 생성 완료\n");
        printf("서버 : 공유 메모리 ID - %d\n", shm_id);
    }
    // 공유메모리 첨부
    if ((void *)-1 == (shm_ptr = (struct shiritori*)shmat(shm_id, (void *)0, 0))){
        printf("서버 : ERROR - 공유 메모리 첨부 실패\n");
        exit(1);
    }else{
        printf("서버 : 공유 메모리 첨부 완료\n");
    }
    shm_ptr->server_pid = pid;
    // 랜덤 제시어 생성
    pthread_create(&start, NULL,*MakeStartWord,NULL);
    pthread_join(start,NULL);
    printf("========================================\n");
    printf("클라1 로부터의 응답 대기중 ...\n");
    shm_ptr->flag = 1;
     // 반복문 필요 구간
    while(shm_ptr->flag){
        pause(); 
        kill(shm_ptr->client2_pid,SIGUSR1);
        pause();
        kill(shm_ptr->client1_pid,SIGUSR1);
    }
    printf("끝말잇기가 종료되었습니다.\n");
    pthread_mutex_destroy(&mutex);
    shmdt((void*)shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
    // pthread_mutex_destroy(&mutex);
}

void *MakeStartWord(){
    srand(time(NULL));
    int a; 
    a = rand() % 5;
    pthread_mutex_lock(&mutex);
    printf("========================================\n");
    printf("랜덤 제시어는 : %s \n",Start_word[a]);
    snprintf((char*)shm_ptr->word, sizeof(shm_ptr->word),"%s",Start_word[a]);
    snprintf((char*)shm_ptr->Lastword, sizeof(shm_ptr->Lastword),"%s",(char*)shm_ptr->word);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void FromClient1(int signum){
    if(signum == SIGUSR1){
        char word[50];
        pthread_mutex_lock(&mutex);
        snprintf(word, sizeof(word),"%s",(char*)shm_ptr->word);
        if(word != "/ff"){
            printf("========================================\n");
            printf("마지막 단어 : %s\n", (char*)shm_ptr->Lastword);
            // printf("마지막 단어의 끝문자 : %s")
            printf("클라이언트1의 단어 : %s",(char*)shm_ptr->word);
            snprintf((char*)shm_ptr->Lastword, sizeof(shm_ptr->Lastword),"%s",(char*)shm_ptr->word);
            printf("========================================\n");
            printf("클라2 로부터의 응답 대기중 ...\n");
        }else if (word == "/ff\n"){
            printf("클라이언트2의 승리\n");
            shm_ptr->flag = 0;
        }
        pthread_mutex_unlock(&mutex);
    }
}

void FromClient2(int signum){
    if(signum == SIGUSR2){
        char word[50];
        pthread_mutex_lock(&mutex);
        snprintf(word, sizeof(word),"%s",(char*)shm_ptr->word);
        if(word != "/ff"){
            printf("========================================\n");
            printf("마지막 단어 : %s\n", (char*)shm_ptr->Lastword);
            printf("클라이언트2의 단어 : %s",(char*)shm_ptr->word);
            snprintf((char*)shm_ptr->Lastword, sizeof(shm_ptr->Lastword),"%s",(char*)shm_ptr->word);
            printf("========================================\n");
            printf("클라1 로부터의 응답 대기중 ...\n");
        }else if(word=="/ff\n"){
            printf("클라이언트1의 승리\n");
            shm_ptr->flag = 0;
        }
        pthread_mutex_unlock(&mutex);
    }
}