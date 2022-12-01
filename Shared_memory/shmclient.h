#ifndef __SHMCLIENT_H__
#define __SHMCLIENT_H__
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

#endif