#include<stdio.h>

int A(int x) {
	return !~x;
}

int B(int x) {
	return !!x;
}



int main (int argc, char* argv[]) {
	printf("%d", A());
	printf("%d", B());
	return 0;
}
