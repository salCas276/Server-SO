#include <stdio.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>


int main(){
	int fd = open("testFile", O_RDWR);
	dup2(fd, 1);
	dup2(fd, 2);
    char* argv2[] = {"./server", NULL};
    execv(argv2[0], argv2); 
}

    
