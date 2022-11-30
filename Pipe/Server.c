#define _POSIX_SOURCE
#include <sys/stat.h>
#include<stdio.h> 
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

char readStr[256];
bool firstword_used;
int user_connect = 0;
pthread_t pthread1[3];
int winner=0;
bool client_bool,sig_timeout;


void *p_write_data(void* word){//파일에 저장
    char *data = word;
    //파일 락
    FILE* fo = fopen("word_arr.txt","a");
    fprintf(fo,"%s ",data);
    //파일 언락
    fclose(fo);
   	pthread_exit(NULL);

}

void *p_timer(){//타이머 스레드
    int endTime = (unsigned)time(NULL); //타이머 선언
    endTime+= 10; //초 제한
    while(1){
        int startTime = (unsigned)time(NULL);
        printf("현재 카운트 시간 : %d\n",endTime-startTime);
        if(endTime-startTime ==0){
            break;
        }
        sleep(1);
    }
    sig_timeout=true;
    pthread_exit(NULL);
}
void *p_read_client(void* client){ //클라이언트로부터 데이터읽기
    char *filepath = client;
    int fd;
    fd=open(filepath,O_RDWR);
    if(fd==-1){
        printf("Error\n");
        return 0;
    }
    //성능 측정 타이머 시작
    read(fd,readStr,sizeof(readStr));
    printf("%s\n",readStr);
    //성능 측정 타이머 종료
    if(strstr(readStr,"입장")>0){
        printf("사용자 1명 접속\n");
        strcpy(readStr,"없음");
        user_connect++;
    }else{
        pthread_cancel(pthread1[1]);
        pthread_create(&pthread1[0],NULL,p_write_data,(void *) readStr);
        pthread_join(pthread1[0],NULL);
        client_bool=!client_bool;
    }
   	pthread_exit(NULL);

}

int main()
{   
    
    //0번 쓰레드는 입력을 받은 것을 파일에 쓰는 스레드
    //1번 쓰레드는 시간을 책정해 사용자가 해당 입력 시간을 넘기면 2번 스레드를 종료시킴
    //2번 쓰레드는 사용자로부터 입력을 받는 스레드
    bool tt=false;
    char str1[100];
    char data[100];
    int fd1, fd2, fd3, fd4;
	int file1 = mkfifo("myfifo1",0666); //클라이언트1에게 보낼때 쓰는 파일
	int file2 = mkfifo("myfifo2",0666); //클라이언트1에게 받을때 쓰는 파일
    int file3 = mkfifo("myfifo3",0666); //클라이언트2에게 보낼때 쓰는 파일
	int file4 = mkfifo("myfifo4",0666); //클라이언트2에게 받을때 쓰는 파일
    int result;
    char sendtext_cli[100];
    char win_cli[100];
    char filePath[100];
    char print_last_word[100];
    fd1 = open("myfifo1",O_RDWR);//클라이언트1에게 보낼때 쓰는 파일
    fd3 = open("myfifo3",O_RDWR);//클라이언트2에게 보낼때 쓰는 파일

    printf("서버 on\n");

    printf("사용자의 접속을 기다리는 중입니다.\n");
    strcpy(filePath,"myfifo2");
    pthread_create(&pthread1[2],NULL,p_read_client,(void *)filePath);
    pthread_join(pthread1[2],NULL);
    strcpy(filePath,"myfifo4");
    pthread_create(&pthread1[2],NULL,p_read_client,(void *)filePath);
    pthread_join(pthread1[2],NULL);

    if(user_connect==2){
        printf("게임을 시작합니다\n");
    }  
    
    while(1){
        sleep(3);
        if(!client_bool){
            strcpy(print_last_word,"클라이언트1님 차례, 마지막단어는 : ");
            strcpy(sendtext_cli,strcat(print_last_word,readStr));
            write(fd1,sendtext_cli,sizeof(sendtext_cli));
            strcpy(filePath,"myfifo2");
            pthread_create(&pthread1[1],NULL,p_timer,NULL);
            pthread_create(&pthread1[2],NULL,p_read_client,(void *)filePath);
            if(pthread_join(pthread1[1],NULL)==0&&sig_timeout){
                pthread_cancel(pthread1[2]);
                winner=2;
                break;
            }
            

        }else{
            strcpy(print_last_word,"클라이언트2님 차례, 마지막단어는 : ");
            strcpy(sendtext_cli,strcat(print_last_word,readStr));
            write(fd3,sendtext_cli,sizeof(sendtext_cli));
            strcpy(filePath,"myfifo4");
            pthread_create(&pthread1[1],NULL,p_timer,NULL);
            pthread_create(&pthread1[2],NULL,p_read_client,(void *)filePath);

            if(pthread_join(pthread1[1],NULL)==0&&sig_timeout){
                pthread_cancel(pthread1[2]);
                winner=1;
                break;
            }
            
            
        }
    }
    if(winner==1){
        strcpy(win_cli,"클라이언트 1님의 승리");
        write(fd1,win_cli,sizeof(win_cli));
        strcpy(win_cli,"클라이언트 2님의 패배");
        write(fd3,win_cli,sizeof(win_cli));
    }else if(winner==2){
        strcpy(win_cli,"클라이언트 1님의 패배");
        write(fd1,win_cli,sizeof(win_cli));
        strcpy(win_cli,"클라이언트 2님의 승리");
        write(fd3,win_cli,sizeof(win_cli));
    }
    printf("게임을 종료합니다.\n");
    close(fd1);
    close(fd3);
    
}