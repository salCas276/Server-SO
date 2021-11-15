CC = gcc
CFLAGS = -g -Wall
SERVER_NAME= server

all: client segment

segment: server
	$(CC) $(CFLAGS) section.c -o section
	objcopy $(SERVER_NAME) --add-section .RUN_ME=section 

server: 
	$(CC) $(CFLAGS) server.c -o $(SERVER_NAME) -lm

client:
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f client server section
