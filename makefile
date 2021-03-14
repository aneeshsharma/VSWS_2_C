CC = gcc

all: server.o

server.o: server.c
	$(CC) server.c -lpthread -o server.o

clean:
	rm server.o