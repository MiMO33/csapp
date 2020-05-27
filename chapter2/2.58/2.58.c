#include<stdio.h>

int is_little_end() {
	int x = 1;
	char y = *((char*) &x);
	return y == 1;
}

int main(int argc, char* argv[]) {
	int ret = is_little_end();
	if(ret == 1)
		printf("true\n");
	else
		printf("false\n");
	return 0;
}
