#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>

#define KEY_NUM 60062

// 끝말잇기 구조체
struct shiritori {
    char word[50];
    char Lastword[50];
    int flag, server_pid, client1_pid, client2_pid;
};

struct shiritori* shm_ptr;

int shm_id;
char* lastword(char* word);
int main(){
    if (-1 == (shm_id = shmget((key_t)KEY_NUM, sizeof(struct shiritori), IPC_CREAT | 0666))){
        printf("클라2 : ERROR - 공유 메모리 접근 실패\n");
        return -1;
    }else{
        printf("클라2 : 공유 메모리 접근 완료\n");
    }   

    // 공유 메모리 첨부
    if ((void *)-1 == (shm_ptr = shmat(shm_id, (void *)0, 0))){
        printf("클라2 : ERROR - 공유 메모리 첨부 실패\n");
    }else{
        printf("클라2 : 공유 메모리 첨부 완료\n");
    }
    
    snprintf((char*)shm_ptr->word, sizeof(shm_ptr->word),"%s","말자하");
    snprintf((char*)shm_ptr->Lastword, sizeof(shm_ptr->word),"%s","얼룩말");
    // strcpy(shm_ptr->Lastword,"얼룩말");
    char *word = (char*)shm_ptr->word;
    char FirstWord[3] = {word[0],word[1],word[2]};
    char *LastWord = lastword((char*)shm_ptr->Lastword); 
    printf("%s\n",FirstWord);
    printf("%s\n",LastWord);

    if(strcmp(FirstWord, LastWord)==0){
        printf("맞았습니다\n");
    }else{
        printf("X\n");
    }
    if(-1 == shmdt((void*)shm_ptr)){
        printf("클라1 : ERROR - 공유 메모리 해제 실패\n");
    }else{
        printf("클라1 : 공유 메모리 해제 완료");
    }
    return 0;
}

char *lastword(char *word){
    int i = 0;
    char *last;
    for(i=0; i < sizeof(word)+2 ; i++){
        if ( word[i] == '\0'){
            char str[3] = {word[i-3], word[i-2],word[i-1]};
            printf("last word 함수 출력 :%c%c%c\n",word[i-3], word[i-2],word[i-1]);
            strcpy(last,str);
        }
    }
    return last;
}