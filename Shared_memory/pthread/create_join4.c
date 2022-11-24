// 1. threadID라는 스레드 변수를 생성

// 2. 스레드 생성, 이때 threadID를 인자로 보내어 TID를 받아낸다. 그리고 threadRoutine라는 함수로 스레드를 실행한다.

// 3. (void*)argument를 통해 argument를 스레드가 실행하는 함수에 보낸다. 

// 4. join에 의해 스레드를 기다린다.

// 5. argumentPointer로 받은 값을 int argument에 값을 저장하고 + 10을하여 return한다.

// 6. 자식 스레드가 종료되고 난 뒤, 메인 스레드는 마지막으로 스레드가 정상적으로 생성 되었었는지 return value를 통해 확인한다.

// 7. join함수의 2번째 인자인 value에 자식 스레드에서 리턴이 제대로 왔는지 printf를 통해 확인한다
        // value값에는 해당 쓰레드의 return 값이 들어있다.
#include <pthread.h>
#include <stdio.h>

void *threadRoutine(void *argumentPointer)
{
    long argument;
    argument = *((long *)argumentPointer);
 
    // 부모 스레드 부분에서 리턴값을 받기때문에 항상 리턴을 해준다.
    return (void*)(argument + 10);
}
int main(){

    pthread_t threadID;

     // threadID로 TID를 받아오고, threadRoutine라는 함수 포인터로 스레드를 실행한다.
    printf("Create Thread!\n");

    // 알규먼트를 생성하여 스레드에 보내준다.
    long argument = 1;
    int ret;
    void *value;

    ret = pthread_create(&threadID, NULL, threadRoutine, (void*)&argument);

    // threadID를 가진 thread가 실행되는 동안 기다린다.
    printf("Main Thread is now waiting for Child Thread!\n");
 
    pthread_join(threadID, &value);

    printf("Main Thread finish waitng Child Thread!\n");

    printf(" == Return Value (0 : success , others : fail) :: %d\n",ret);
    printf(" == Receive Value :: %ld\n", (long)value); 
 
    return 0;
}