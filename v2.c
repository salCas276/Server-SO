#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
       #include <netinet/in.h>
       #include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main(){
    int servFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_port = htons(8080);
    servAdd.sin_addr.s_addr = inet_addr("0.0.0.0");

    connect(servFd, (struct sockaddr *) &servAdd, 16);
    sleep(60);

    char buff[1024];
    strcpy(buff, "entendido\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "itba\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "M4GFKZ289aku\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "fk3wfLCm3QvS\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "too_easy\n");
    write(3, buff, strlen(buff));
    strcpy(buff, ".RUN_ME\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "K5n2UFfpFMUN\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "BUmyYq5XxXGt\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "u^v\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "chin_chu_lan_cha\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "gdb_rules\n");
    write(3, buff, strlen(buff));
    strcpy(buff, "normal\n");
    write(3, buff, strlen(buff));
    
    return 0;
}
