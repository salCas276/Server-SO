
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
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>




#define PORT_NUMBER 8080
#define MAX_CONNECTION 5
#define MAX_ANSWER 128 
#define CHALLENGES_NUMBER 12
#define BAD_FD 13
#define HASH_MD5_LENGTH 32

void ebadf(void);
void killTracer(void);
void quine(void);
void gdbme(void);
void mixOutput(void);
void printResalted(void);
void gdbme(void);
int checkAnswer(char * answer , int index);


char * challengesDescription[CHALLENGES_NUMBER]={"Desafio 1\n","Desafio 2\n","Desafio 3\n","Desafio 4\n","Desafio 5\n","Desafio 6\n","Desafio 7\n","Desafio 8\n","Desafio 9\n","Desafio 10\n","Desafio 11\n","Desafio 12\n"};
char * challengesAnswer[CHALLENGES_NUMBER]={"eb112daf85b0e0fd7c662a23e53cd7b3","832b9cae42ea0a246e90bee87edbb0e9","21fa82834f219bac1780d58487e6b419","b4347457f90783ee0a98ffc30a2bed4f","abb49e03cdcd8f5c8e49372bb98d2896","c9206e7503c1faa8189857e1feca66dc","cb77b14184d6d1cfb63532c38ff0c511","dbe4a7848abca6fd0cabe4a6303da354","f2bf16ef393127b97de8e4c4dab85480","f9684b2fe6ca62e27329ff00fd95ed45","751096e6b1674f5203d75431965d8a75","2f3c3dd012fff07506bc8e641ab7ba13"};
void (*challengePreparation[CHALLENGES_NUMBER])(void)={NULL,NULL,NULL,ebadf,NULL,NULL,mixOutput,printResalted,NULL,quine,gdbme,NULL};
char * challengesAnswerPlane[CHALLENGES_NUMBER]={"entendido\n","itba\n","M4GFKZ289aku\n","fk3wfLCm3QvS\n","too_easy\n",".RUN_ME\n","K5n2UFfpFMUN\n","BUmyYq5XxXGt\n","u^v\n","chin_chu_lan_cha\n","gdb_rules\n","normal\n"};
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

	system("clear");
	printf("%s",challengesDescription[*currentChallenge]);

	if(challengePreparation[*currentChallenge] != NULL ){
		challengePreparation[*currentChallenge]();
	}

	char answer[MAX_ANSWER];
	if(fgets(answer, MAX_ANSWER , socketFP ) != NULL){
		if( !checkAnswer(answer,*currentChallenge) ){
			*currentChallenge=(*currentChallenge + 1) ; 
			printf("Respuesta Correcta \n");
		}
		else {
			printf("Respuesta Incorrecta \n");
		}
	}else { //eof 
		*currentChallenge = -1;
	}
	 usleep(100000);
}



int checkAnswer(char * answer , int index){
	int pipefd[2];
	if( pipe(pipefd) < 0){
		return -1;
	}

		int stdoutCopy = dup(STDOUT_FILENO);
		if(dup2(pipefd[1],STDOUT_FILENO) < 0)
			return -1;	
		
		close(pipefd[1]);
		
		FILE * hashProcess = popen("md5sum | cut -c1-32","w");
		if(hashProcess == NULL)
			return -1;


		int hashProcessfd = fileno(hashProcess);
		
		if ( write(hashProcessfd,answer,strlen(answer)) < 0 ) 
			return -1;
		
		pclose(hashProcess);
		
		if( dup2(stdoutCopy,STDOUT_FILENO) < 0 ) 
			return -1; 
		
		close(stdoutCopy);

		
		char buffer[HASH_MD5_LENGTH];
		
		if ( read(pipefd[0],buffer,HASH_MD5_LENGTH) < 0 ) 
			return -1;
		
		buffer[HASH_MD5_LENGTH]=0;
		
		close(pipefd[0]);
		
		return strcmp(buffer,challengesAnswer[index]);
}





void ebadf(void){

	char respuesta[90];
	strcpy(respuesta,"................................................. la respuesta es:");
	strcat(respuesta,challengesAnswerPlane[4]);
	write(BAD_FD,respuesta,strlen(respuesta));

	int stderrAux = dup(2);
	char * errMssg="write: Bad file descriptor\n";
	write(stderrAux,errMssg,strlen(errMssg));
	close(stderrAux);
}

void mixOutput(void){
	killTracer();
	char * trash = "AAajshdfbeaiodDSF__ASDHFAEO{{1↨1#1#1│5¤5ù±23{1↨1↨1þ23☺23☺231-48484♠5847♣1♠47♣";
	char * rta = malloc(60);
	strcpy(rta,"La respuesta es ");
	strcat(rta,challengesAnswerPlane[6]);


	int i = 0 ; 
	int j = 0 ; 
	while(trash[i] || rta[j]){
		if(trash[i]){
			write(STDERR_FILENO,trash+i,1);
			i++;
		}
		if(rta[j]){
			write(STDOUT_FILENO,rta+j,1);
			j++;
		}
	}
	free(rta);
	printf("\n");

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






void quine(void){
	if( system("gcc -o quine quine.c") == -1 ){
		fprintf(stderr,"error:%s\n",strerror(errno));
		return;
	}  


	int tempfd=open("temp",O_RDWR|O_CREAT,S_IRWXU);
	if(tempfd < 0 )
		return;
	
	pid_t sonPid = fork();
	if(sonPid){
		wait(NULL);
		int retorno=system("diff quine.c temp");
		if(!retorno){
			printf("La respuesta es %s",challengesAnswerPlane[9]);
		}else{
			printf("Segui intentando\n");
		}
		remove("temp");
		return;
	}else{
		dup2(tempfd,1);
		close(tempfd);
		char * argv[]={"./quine",NULL};
		execv(argv[0],argv);
	}
}

void printResalted(void){
	char * comando =  malloc(60);
	strcpy(comando,"\033[48;5;17m\e[38;5;17m");
	strcat(comando, "La respuesta es: ");
	strcat(comando,challengesAnswerPlane[7]);
	strcat(comando,"\e[0m");
	printf("%s",comando);
	free(comando);
}




void gdbme(void){
	pid_t pid = getpid(); 
	if(pid == 0x12345678){
		printf("La respuesta es %s\n",challengesAnswerPlane[10]);
	}else {
		printf("Enter para reintentar\n");
	}
}