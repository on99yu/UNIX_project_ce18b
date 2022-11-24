// 1. threadID라는 스레드 변수를 생성
// 2. 스레드 생성, 이때 threadID를 인자로 보내어 TID를 받아낸다. 그리고 threadRoutine라는 함수로 스레드를 실행한다.
// 3. (void*)argument를 통해 argument를 스레드가 실행하는 함수에 보낸다. 
// 4. join에 의해 스레드를 기다린다.

// 5. argumentPointer로 받은 값을 int argument에 값을 저장하고 + 10을하여 출력한다.

// 6. 자식 스레드가 종료되고 난 뒤, 메인 스레드는 마지막으로 스레드가 정상적으로 생성 되었었는지 return value를 통해 확인한다.
//      0이면 성공 다른값이면 실패
#include <pthread.h>
#include <stdio.h>

void *threadRoutine(void *argumentPointer)
{
    long argument;
    argument = *((long *)argumentPointer);
 
    printf("%ld\n", argument + 10 );
    // 부모 스레드 부분에서 리턴값을 받기때문에 항상 리턴을 해준다.
    return NULL;
}

int main(){

     pthread_t threadID;
    
    // threadID로 TID를 받아오고, threadRoutine라는 함수 포인터로 스레드를 실행한다.
    printf("Create Thread!\n");

     // 알규먼트를 생성하여 스레드에 보내준다.
    long argument = 1;
    int ret;
    ret = pthread_create(&threadID, NULL, threadRoutine, (void*)&argument);

        // threadID를 가진 thread가 실행되는 동안 기다린다.
    printf("Main Thread is now waiting for Child Thread!\n");

    pthread_join(threadID, NULL);
 
    printf("Main Thread finish waitng Child Thread!\n");

      printf("Return Value (0 : success , others : fail) :: %d\n",ret);
 
    return 0;
    

}