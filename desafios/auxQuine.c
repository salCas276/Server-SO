#include <stdio.h>

int main(){
	printf("#include <unistd.h>\n\nint main(){\nchar* aux[] = {\"./auxQuine\", 0};\nexecv(aux[0], aux);\nreturn 0;\n}\n\n");
	return 0;
}
