#include <unistd.h>

int main(){
char* aux[] = {"./auxQuine", 0};
execv(aux[0], aux);
return 0;
}

