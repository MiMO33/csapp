#include<stdlib.h>
#include"show-bytes-more.h"

void show_num(int num) {
	show_int(num);
	show_float((float)num);
	show_pointer(&num);
	show_double((double)num);
	show_short((short)num);
	show_long((long)num);
}

int main(int argc, char* argv[]){
	if(argc != 2) {
		fprintf(stderr, "Usage: %s num\n",
		argv[0]);
	exit(1);
	}
	
	int num = atoi(argv[1]);
	show_num(num);
	return 0;
}
