long decode2(long x, long y, long z) {
	y = y - x;
	z = z * y;
	long i = y;
	i << 63;
	i >> 63;
	i = i ^ x;
	return i;
}
