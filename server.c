
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>




#define PORT_NUMBER 8080
#define MAX_CONNECTION 5
#define MAX_ANSWER 128 
#define CHALLENGES_NUMBER 12
#define BAD_FD 13

void ebadf(void);
void killTracer(void);



char * challengesDescription[CHALLENGES_NUMBER]={"Desafio 1\n","Desafio 2\n","Desafio 3\n","Desafio 4\n","Desafio 5\n","Desafio 6\n","Desafio 7\n","Desafio 8\n","Desafio 9\n","Desafio 10\n","Desafio 11\n","Desafio 12\n"};
char * challengesAnswer[CHALLENGES_NUMBER]={"entendido\n","itba\n","M4GFKZ289aku\n","fk3wfLCm3QvS\n","too_easy\n",".RUN_ME\n","K5n2UFfpFMUN\n","BUmyYq5XxXGt\n","u^v\n","chin_chu_lan_cha\n","gdb_rules\n","normal\n"};
void (*challengePreparation[CHALLENGES_NUMBER])(void)={NULL,NULL,NULL,ebadf,NULL,NULL,killTracer,NULL,NULL,NULL,NULL,NULL};
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

	printf("Buena campeón conseguiste pasar todos los desafios\n");

	return 0 ; 

}



void callChallenge(int * currentChallenge , FILE * socketFP){

	printf("%s",challengesDescription[*currentChallenge]);

	if(challengePreparation[*currentChallenge] != NULL ){
		challengePreparation[*currentChallenge]();
	}

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



void ebadf(void){

	char respuesta[90];
	strcpy(respuesta,"................................................. la respuesta es:");
	strcat(respuesta,challengesAnswer[4]);
	write(BAD_FD,respuesta,strlen(respuesta));

	int stderrAux = dup(2);
	char * errMssg="write: Bad file descriptor\n";
	write(stderrAux,errMssg,strlen(errMssg));
	close(stderrAux);
}




void killTracer(void){
	int pipefd[2];
	if( pipe(pipefd) < 0){
		return ;
	}
	pid_t forkReturn= fork();
	if(forkReturn){
		close(pipefd[1]);
		char buffer[128];
		int tracerPid = atoi( fgets(buffer,128,fdopen(pipefd[0],"r") ) ); 
		if(tracerPid)
			kill(tracerPid, SIGKILL);
		wait(NULL);
		close(pipefd[0]);
		return;
	}else{
		close(pipefd[0]);
		dup2(pipefd[1],STDOUT_FILENO);
		close(pipefd[1]);
		char * tracerPidParser = malloc(90);
		strcpy(tracerPidParser,"grep Tracer /proc/");
		pid_t  ppid = getppid();
		char parentPidbuffer[10];
		sprintf(parentPidbuffer,"%d",ppid);
		strcat(tracerPidParser,parentPidbuffer);
		strcat(tracerPidParser,"/status | grep -o -E '[0-9]+'");

		system(tracerPidParser);
		
		exit(1);
	}






}