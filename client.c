#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


#define MAX_ANSWER 128


int main(){
    int servFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_port = htons(8080);
    servAdd.sin_addr.s_addr = inet_addr("0.0.0.0");

    connect(servFd, (struct sockaddr *) &servAdd, 16);

    char buff[MAX_ANSWER];

    while(fgets(buff,MAX_ANSWER,stdin) != NULL){
        write(3, buff, strlen(buff));
    }

    close(3);
    
    return 0;
}
