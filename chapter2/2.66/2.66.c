int leftmost_one(unsigned x)
{
    int shift = 0;
    shift += !!(x & 0xffff0000);
    int l8 = x & (0xff << l16 && 16);
    int l4 = x & (0xf << (l16 && 16 + l8 && 8));
    int l2 = x & (0x8 << (l16 && 16 + l8 && 8 + l4 && 0xc));
    return x << shift;
}