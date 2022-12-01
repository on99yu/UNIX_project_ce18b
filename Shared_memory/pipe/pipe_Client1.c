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
bool write_data;
bool word_already;
bool timer,sig_timeout;
pthread_mutex_t mutexsum;

void *p_read_server(void* server){ //서버로부터 값 읽기
    char *filepath = server;
    int fd;
    fd=open(filepath,O_RDWR);
    if(fd==-1){
        printf("Error\n");
        return 0;
    }
    read(fd,readStr,sizeof(readStr));
    printf("%s\n",readStr);

	if(strstr(readStr,"차례")>0){
			write_data=true;
	}

	pthread_exit(NULL);
}

void *p_find_sameword(void* word){
	char *findWord = word;
	char buf[1024];
    pthread_mutex_lock(&mutexsum);
	FILE* fp = fopen("word_arr.txt","r");
	if(fp==NULL){
		pthread_exit(NULL);
	}
	while(!feof(fp)){
		fgets(buf, sizeof(buf) , fp);
		
		char *ptr = strtok(buf, " ");
		while (ptr != NULL) { 
            //printf("자른 문자열 출력 : %s \n" , ptr);
            //두 문자열이 같은지 비교. 같으면  0
            int ret = strcmp(findWord , ptr);
            
            //두 문자열이 같다.
            if (ret == 0) {
                word_already=true;
                printf("중복된 단어가 있습니다 \n");
				break;
            }
            ptr = strtok(NULL, " "); //다음 문자열을 잘라서 포인터를 반환
        }
	}
	fclose(fp);
    pthread_mutex_unlock(&mutexsum);
}

void *p_timer(){
	timer=true;
    int endTime = (unsigned)time(NULL); //타이머 선언
    endTime+= 10; //10초 제한
    while(1){
        int startTime = (unsigned)time(NULL);
        if(endTime-startTime<=3){
			printf("현재 카운트 시간 : %d\n",endTime-startTime);
		}
        if(endTime-startTime ==0){
            break;
        }
        sleep(1);
    }
	timer=false;
	sig_timeout=true;
   	pthread_exit(NULL);
}

int main()
{
	pthread_t pthread1[3];
	//0번 스레드는 서버로부터 값을 읽는 스레드
	//1번 스레드는 중복된 단어가 있는지 파일로부터 찾는 스레드
	//2번 스레드는 타이머 스레드
	int file1, file2;
    int user_connect = 0;
	char str[100];
	char serverFile[30]="myfifo1";
	pthread_mutex_init(&mutexsum,NULL);


	file1= open("myfifo2",O_RDWR);//보낼때 쓰는 파일
	strcpy(str,"클라이언트 입장");
	write(file1,str,sizeof(str));

	//성능 측정을 위한 메시지 주고받기

	file2= open("myfifo1",O_RDWR);
	read(file2, readStr,sizeof(readStr));
	printf("서버로부터 입력받은 값 : %s\n",readStr);
	strcpy(str,"클라이언트1 전송");
	write(file1,str,sizeof(str));

	if(file1==-1){
		printf("error\n");
	}else{
		while(1){
			pthread_create(&pthread1[0],NULL,p_read_server,(void *) serverFile);
			pthread_join(pthread1[0],NULL);
			if(write_data){
				char inputStr[50];
				timer=true;
				pthread_create(&pthread1[2],NULL,p_timer,NULL);
				while(1){
					if(timer==false){
						printf("시간 초과!\n");
						break;
					}
					printf("입력하시오 : ");
					scanf("%s",inputStr);
					
					pthread_create(&pthread1[1],NULL,p_find_sameword,(void *) inputStr);
					pthread_join(pthread1[1],NULL);
					if(word_already){//해당 단어가 이미 있을 경우
						word_already=false;
						continue;
					}else{
						write(file1,inputStr,sizeof(inputStr));
						write_data=false;
						word_already=false;
						printf("당신이 입력한 값은 = %s\n",inputStr);
						pthread_cancel(pthread1[2]);
						break;
					}
				}
			}
			if(sig_timeout){
				pthread_create(&pthread1[0],NULL,p_read_server,(void *) serverFile);
				pthread_join(pthread1[0],NULL);
				break;
			}
		}
		
	}
	exit(0);
	close(file1);

}