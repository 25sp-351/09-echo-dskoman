all: client server

CC = gcc
CFLAGS = -Wall -g

client: client.o
	$(CC) -o client.out $(CFLAGS) client.o  -lm

server: server.o
	$(CC) -o server.out $(CFLAGS) server.o  -lm

client.o: client.c

server.o: server.c