#include<stdio.h>

typedef unsigned char* byte_pointer;

int replace_byte (unsigned x, int i, unsigned char b) {
	byte_pointer byte_start = (byte_pointer) &x;
	byte_start[i] = b;
	return x;
}

int main(int argc, char* argv[]) {
	printf("%x\n", replace_byte(0x12345678, 2, 0xab));
	printf("%x\n", replace_byte(0x12345678, 0, 0xab));
	return 0;
}
	
