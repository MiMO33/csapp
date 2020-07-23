#include<stdio.h>
#include<stdlib.h>

int generate_a_word(int x, int y) {
	int mask_x = 0x000000ff;
	int mask_y = 0xffffff00;
	int ret = (mask_x & x) | (mask_y & y);
	return ret;
}

int main(int argc, char* argv[]) {
	printf("%x \n", generate_a_word(0x89ABCDEF, 0x76543210));
	return 0;	
}
