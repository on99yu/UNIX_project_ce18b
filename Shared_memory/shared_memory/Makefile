CC = gcc
CFLAGS = -lpthread
SHM_C1 = shm_Client1.c
SHM_C2 = shm_Client2.c
SHM_S = shm_Server.c

build : $(SHM_C1) $(SHM_C2) $(SHM_S)
	$(CC) -o shm_Client1 $(SHM_C1) $(CFLAGS)
	$(CC) -o shm_Client2 $(SHM_C2) $(CFLAGS)
	$(CC) -o shm_Server $(SHM_S) $(CFLAGS)
clean :
	rm shm_Server shm_Client1 shm_Client2