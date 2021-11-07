#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define PORT_NUMBER 8080
#define MAX_CONNECTION 5 


int main(){

	int socketFd = socket(AF_INET,SOCK_STREAM,0);
	if(socketFd < 0 ) {
		perror("socket creation");
		return -1;	
	}
	
	int enable = 1 ; 
	if(setsockopt(socketFd,SOL_SOCKET,SO_REUSEPORT,&enable,sizeof(int)) < 0 ) {
		perror("Socket configuration:");
		return -1;
	}


	 struct in_addr address = {	 inet_addr("0.0.0.0") };
	 struct sockaddr_in  addressFormat = {AF_INET , htons(PORT_NUMBER) , address};

	if(bind(socketFd, (const struct sockaddr *) &addressFormat,sizeof(struct sockaddr_in)) < 0 ){
		perror("Socket binding");
		return -1;
	} 

	if ( listen(socketFd,MAX_CONNECTION) < 0 ){
		perror("Socket listening");
		return -1;
	}

	struct sockaddr_in waitingConnection ;
	socklen_t len = sizeof(struct sockaddr_in);
	accept(socketFd, ( struct sockaddr *)&waitingConnection, &len);
	
	printf("COnnected");

		



}
