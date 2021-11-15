#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


#define MAX_ANSWER 128


int main(int argc , char * argv[]){
    if(argc != 2 )
        return -1; 

    int servFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_port = htons(8080);
    servAdd.sin_addr.s_addr = inet_addr(argv[1]);

    connect(servFd, (struct sockaddr *) &servAdd, 16);

    char buff[MAX_ANSWER];

    while(fgets(buff,MAX_ANSWER,stdin) != NULL){
        write(servFd, buff, strlen(buff));
    }

    close(servFd);
    
    return 0;
}
