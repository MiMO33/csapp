#include <stdio.h>
#include <assert.h>

const int w = sizeof(int) >> 3;

unsigned srl(unsigned x, int k) {

	unsigned xsra = (int) x >> k;
	unsigned mask = (1 << (w-k)) - 1;
	return xsra & mask;
}

int sra(int x, int k ) {

	int xsrl = (unsigned) x >> k;
	int sign = ((1 << (w - 1 - k)) & xsrl) << 1;
	int t = ~((1 << (w - k)) - 1);
	int n = t + (sign ^ (1 << (w - k)));
	return xsrl + n;
}

int main(int argc, char* argv[]) {
  unsigned test_unsigned = 0x12345678;
  int test_int = 0x12345678;

  assert(srl(test_unsigned, 4) == test_unsigned >> 4);
  assert(sra(test_int, 4) == test_int >> 4);

  test_unsigned = 0x87654321;
	test_int = 0x87654321;

	assert (srl (test_unsigned, 4) == test_unsigned >> 4);
	assert (sra (test_int, 4) == test_int >> 4);
  
  return 0;
}

