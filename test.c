#include <stdio.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <signal.h>

void sigHandler(int sig){
    printf("Holaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
    return;
}

int main(){
    struct sigaction sa;
    sa.sa_handler = &sigHandler; 
    sa.sa_flags = SA_RESTART;
    sigaction(SIGKILL, &sa, NULL);
    dup2(1, 13);
    char* argv2[] = {"./server", NULL};
    execv(argv2[0], argv2); 
}

    
