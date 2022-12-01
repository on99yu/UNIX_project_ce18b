shm.out : shm_Server.o shm_Client1.o shm_Client2.o
	gcc -o shm.out shm_Server.o shm_Client1.o shm_Client2.o
shm_Server.o : shmclient.h shm_Server.c
	gcc -c -o shm_Server.o shm_Server.c -lpthread
shm_Client1.o : shmclient.h shm_Client1.c
	gcc -c -o shm_Client1.o shm_Client1.c -lpthread
shm_Client2.o : shmclient.h shm_Client2.c
	gcc -c -o shm_Client2.o shm_Client2.c -lpthread
