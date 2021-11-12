CC = gcc
CFLAGS = -g -Wall 
SERVER_NAME= server

all: server client segment 

server: 
segment:
	$(CC) $(CFLAGS) section.c -o section
	objcopy $(SERVER_NAME) --add-section .RUN_ME=section 
client:
	$(CC) $(CFLAGS) client.c -o client
	$(CC) $(CFLAGS) server.c -o $(SERVER_NAME)
clean:
	rm -f client server section
