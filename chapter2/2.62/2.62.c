#include<stdio.h>

int int_shift_are_arithmetic() {
	
	int w = sizeof(int) >> 3;
	int x = 1;
	x << w - 1;
	return (x >> w - 1) == 1;
}
