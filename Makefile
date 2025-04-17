all: server

CC = gcc
CFLAGS = -Wall -g

server: server.o
	$(CC) -o server.out $(CFLAGS) server.o  -lm

server.o: server.c