CC = gcc
CFLAGS = -lpthread
PIPE_C1 = pipe_Client1.c
PIPE_C2 = pipe_Client2.c
PIPE_S = pipe_Server.c

build : $(PIPE_C1) $(PIPE_C2) $(PIPE_S)
	$(CC) -o pipe_Client1 $(PIPE_C1) $(CFLAGS)
	$(CC) -o pipe_Client2 $(PIPE_C2) $(CFLAGS)
	$(CC) -o pipe_Server $(PIPE_S) $(CFLAGS)
clean :
	rm pipe_Server pipe_Client1 pipe_Client2