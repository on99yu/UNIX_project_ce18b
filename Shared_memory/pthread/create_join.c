// 1. threadID라는 쓰레드변수를 생성한다.
// 2. 쓰레드 생성, 이때 threadID를 인자로 보내어 TID를 받아낸다. 그리고 threadRoutine이라는 함수로 스레드를 실행한다.
// 3. join에 의해 쓰레드를 기다린다.
// 4. threadRoutine에서 시작하는 자식 쓰레드는 TID를 출력한다.
// 5. 자식쓰레드가 종료되고 난뒤  메인쓰레드는 마지막으로 메세지를 출력하고 종료한다.
#include <pthread.h>
#include <stdio.h>

void *threadRoutine(void* argumentPointer){

    pthread_t id = pthread_self();

    //TID를 반환하고 출력
    printf("thread ID(TID) :: %lu\n",id);

    // 부모 스레드 부분에서 리턴값을 받기때문에 항상 리턴을 해준다.
    return NULL;
}

int main(){
    pthread_t threadID;

    //threadID로 TID를 받아오고, threadRoutine이라는 함수 포인터로 스레드를 실행한다.
    printf("Create Thread!\n");
    pthread_create(&threadID, NULL, threadRoutine, NULL);

    // threadID를 가진 thread가 실행되는 동안 기다린다.
    printf("Main Thread is now waiting for Child Thread\n");

    pthread_join(threadID, NULL);

    printf("Main Thread finish waiting Child Thread!\n");

    return 0;
}