#include<stdio.h>

int main() {
	
	int x = 0x87654321;
	x = (x & 0xFF) | (x ^ (~0xFF));
	printf("%x", x);
	return 0;
}
