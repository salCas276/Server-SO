#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT_NUMBER 8080
#define MAX_CONNECTION 5
#define MAX_ANSWER 128 
#define CHALLENGES_NUMBER 12


char * challengesDescription[CHALLENGES_NUMBER]={"Desafio 1\n","Desafio 2\n","Desafio 3\n","Desafio 4\n","Desafio 5\n","Desafio 6\n","Desafio 7\n","Desafio 8\n","Desafio 9\n","Desafio 10\n","Desafio 11\n","Desafio 12\n"};
char * challengesAnswer[CHALLENGES_NUMBER]={"entendido\n","itba\n","M4GFKZ289aku\n","fk3wfLCm3QvS\n","too_easy\n",".RUN_ME\n","K5n2UFfpFMUN\n","BUmyYq5XxXGt\n","u^v\n","chin_chu_lan_cha\n","gdb_rules\n","normal\n"};

void callChallenge(int * currentChallenge , FILE * socketFP);

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
	
	int socketFD = accept(socketFd, ( struct sockaddr *)&waitingConnection, &len);
	FILE* socketFP = fdopen(socketFD, "r");


	int currentChallenge = 0 ; 
	while(currentChallenge < CHALLENGES_NUMBER){
		callChallenge(&currentChallenge,socketFP);
		if(currentChallenge < 0 && currentChallenge != CHALLENGES_NUMBER - 1){
			printf("Game over!!");
			return 1;
		}
	}

	printf("Felicitacion conseguiste pasar todos los desafios\n");

	return 0 ; 

}



void callChallenge(int * currentChallenge , FILE * socketFP){

	printf("%s",challengesDescription[*currentChallenge]);

	//call some function related to the challenge e.g: gdbme.
	
	char answer[MAX_ANSWER];
	if(fgets(answer, MAX_ANSWER , socketFP ) != NULL){
		if( !strcmp(answer,challengesAnswer[*currentChallenge]) ){
			*currentChallenge=(*currentChallenge + 1) ; 
			printf("Respuesta Correcta \n");
			return;
		}
		else {
			printf("Respuesta Incorrecta \n");
			return ; 	
		}
	}else { //eof 
		*currentChallenge = -1;
	}


}
