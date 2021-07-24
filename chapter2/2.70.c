int fits_bits(int x, int n) {
    int w = sizeof(int) << 3;
    int offset = w - n;
    return (x << offset >> offset) == x;
}