#include<stdio.h>

int A(int x) {
	return !~x;
}

int B(int x) {
	return !!x;
}

int C(int x) {
	return (x & 0xFF) == 0xFF;
}

int D(int x) {
	int mask = 0xFF << ((sizeof(int) - 1) << 3);
	return (x ^ mask) == 0;
}

int main (int argc, char* argv[]) {
	
	printf("C: %d\n", C(0xFF));
	printf("D: %d\n", D(0xFF000000));
	return 0;
}
