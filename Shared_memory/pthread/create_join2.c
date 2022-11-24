// 1. threadID라는 쓰레드 변수를 생성
// 2. 쓰레드 생성, threadID를 인자로 보내어 TID를 받아낸다. 그리고 threadRoutine이라는 함수로 쓰레드를 실행한다.
// 3. (void*)argument를 통해 argument를 쓰레드가 실행하는 함수에 보낸다.
// 4. join에 의해 쓰레드를 기다린다.
// 5. argumentPointer로 받은 값을 char* argument가 가리키도록하고 argumentPointer에서 제대로 값이 왔는지 확인한다.
// 6. 자식쓰레드가 종료되고 난뒤, 메인 쓰레드는 마지막으로 printf를 출력하고 종료한다.
#include <pthread.h>
#include <stdio.h>

void *threadRoutine(void *argumentPointer)
{
    char *argument = (char *)argumentPointer;
 
    printf("%s\n", argument );
    // 부모 스레드 부분에서 리턴값을 받기때문에 항상 리턴을 해준다.
    return NULL;
}

int main(){
    pthread_t threadID;

    // threadID로 TID를 받아오고, threadRoutine라는 함수 포인터로 스레드를 실행한다.
    printf("Create Thread!\n");

    // arg를 생성하여 스레드에 보내준다.
    char argument[10] = "hello";

    pthread_create(&threadID, NULL, threadRoutine, (void*)argument);

     // threadID를 가진 thread가 실행되는 동안 기다린다.
    printf("Main Thread is now waiting for Child Thread!\n");

    pthread_join(threadID, NULL);
    printf("Main Thread finish waitng Child Thread!\n");
    
    return 0;
}