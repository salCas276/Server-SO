CC = gcc
CFLAGS = -g -Wall 
SERVER_NAME= serverPropio

all: server client segment 

server: 
segment:
	objcopy --dump-section .RUN_ME=section server
	chmod u+x section
	objcopy $(SERVER_NAME) --add-section .RUN_ME=section 
client:
	$(CC) $(CFLAGS) client.c -o client
	$(CC) $(CFLAGS) server.c -o $(SERVER_NAME)
clean:
	rm -f serverPropio client serverPropio section
