#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY_NUM 600601
#define MEM_SIZE 1024

int main(void)
{
	int shm_id;
	void *shm_addr;
	int count;

	if ((shm_id = shmget((key_t)KEY_NUM, MEM_SIZE, IPC_CREAT | 0666) == -1))
	{
		printf("공유 메모리 생성 실패\n");
		return -1;
	}
	if ((void *)-1 == (shm_addr = shmat(shm_id, (void *)0, 0)))
	{
		printf("공유 메모리 첨부 실패\n");
		return -1;
	}
	count = 0;
	while (1)
	{
		sprintf((char *)shm_addr, "%d", count++);
		sleep(1);
	}
	return 0;
}