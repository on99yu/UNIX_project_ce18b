shm_Server.out : shm_Server.o 
	gcc -o shm_Server.out shm_Server.o -lpthread
shm_Client1.out : shm_Client1.o
	gcc -o shm_Client1.out shm_Client1.o
shm_Client2.out : shm_Client2.o
	gcc -o shm_Client2.out shm_Client2.o
shm_Server.o : shm_Server.c
	gcc -c -o shm_Server.o shm_Server.c -lpthread
shm_Client1.o : shm_Client1.c
	gcc -c -o shm_Client1.o shm_Client1.c -lpthread
shm_Client2.o : shm_Client2.c
	gcc -c -o shm_Client2.o shm_Client2.c -lpthread
