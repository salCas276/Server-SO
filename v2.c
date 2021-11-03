#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
       #include <netinet/in.h>
       #include <arpa/inet.h>


int main(){
    int servFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_port = htons(8080);
    servAdd.sin_addr.s_addr = inet_addr("0.0.0.0");

    connect(3, (struct sockaddr *) &servAdd, 16);
    char buff[] = "entendido\n";
    send(3, buff, 10, 0);
    
    return 0;
}
