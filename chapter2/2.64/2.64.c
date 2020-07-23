#include<assert.h>
#include<stdio.h>

int any_odd_one(unsigned x) {
	
	return x & 0x55555555 == 0x55555555;
}


int main(int argc, char* argv[]) {
  assert(any_odd_one(0x2));
  assert(!any_odd_one(0x4));
  return 0;
}

