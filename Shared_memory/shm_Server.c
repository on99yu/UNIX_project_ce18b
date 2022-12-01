#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define KEY_NUM 60063

// 공유메모리 끝말잇기 구조체
struct shiritori {
    char Recentword[50]; // 가장 최근단어
    char Lastword[50]; // 마지막 입력된 단어
    int flag, server_pid, client1_pid, client2_pid; 
    // 서버 프로세스 ID와 클라이언트들의 프로세스 ID
};
// 구조체 참조 포인터 생성
struct shiritori* shm_ptr;

// 시작 제시어
char *Start_word[50] = {"얼룩말","안경집","마우스","콘센트","기차표"};

// 쓰레드 선언 
pthread_t start, client1, client2;
pthread_mutex_t mutex;
void *MakeStartWord();
void FromClient1(int signum);
void FromClient2(int signum);
char *lastword(char *word);
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
    snprintf((char*)shm_ptr->Recentword, sizeof(shm_ptr->Recentword),"%s",Start_word[a]);
    snprintf((char*)shm_ptr->Lastword, sizeof(shm_ptr->Lastword),"%s",(char*)shm_ptr->Recentword);
    // char *last = RemoveEnd((char*)shm_ptr->Lastword);
    // printf("마지막 단어의 끝 문자: %s\n",last);
    pthread_mutex_unlock(&mutex);
    return NULL;
}
// 클라이언트1로부터 시그널이 왔을때
void FromClient1(int signum){
    if(signum == SIGUSR1){
        pthread_mutex_lock(&mutex);
        char *word = (char*)shm_ptr->Recentword;
        printf("========================================\n");
        printf("마지막 단어 : %s\n", (char*)shm_ptr->Lastword);
        printf("클라이언트1의 단어 : %s",(char*)shm_ptr->Recentword);
        snprintf((char*)shm_ptr->Lastword, sizeof(shm_ptr->Lastword),"%s",(char*)shm_ptr->Recentword);
        printf("========================================\n");
        printf("클라2 로부터의 응답 대기중 ...\n");
        pthread_mutex_unlock(&mutex);
    }
}
// 클라이언트2로부터 시그널이 왔을때
void FromClient2(int signum){
    if(signum == SIGUSR2){
        pthread_mutex_lock(&mutex);
        char *word = (char*)shm_ptr->Recentword;
        // char FirstWord[3] = {word[0],word[1],word[2]};
        // char *LastWord = lastword((char*)shm_ptr->Lastword);
        // printf("%s",lastword((char*)shm_ptr->Lastword));
        // printf("마지막 단어 끝문자 : %s\n",LastWord);
        // printf("최근 단어 첫문자 %s\n",FirstWord);
        printf("========================================\n");
        printf("마지막 단어 : %s\n", (char*)shm_ptr->Lastword);
        printf("클라이언트2의 단어 : %s",(char*)shm_ptr->Recentword);
        snprintf((char*)shm_ptr->Lastword, sizeof(shm_ptr->Lastword),"%s",(char*)shm_ptr->Recentword);
        printf("========================================\n");
        printf("클라1 로부터의 응답 대기중 ...\n");
        pthread_mutex_unlock(&mutex);
    }
}

// char *lastword(char *word){
//     int i = 0;
//     for(i=0; i < sizeof(word)+2 ; i++){
//         if ( word[i] == '\0'){
//             char str[3] = {word[i-3], word[i-2],word[i-1]};
//             snprintf((char*)shm_ptr->Lastchar,sizeof(shm_ptr->Lastchar),"%s",str);
//         }
//     }
// }