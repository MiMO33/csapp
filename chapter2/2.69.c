/*
* Do rotating left shift.
Assume 0 <= n < w
* Examples when x = 0x12345678 and w = 32:
*
n=4 -> 0x23456781, n=20 -> 0x67812345
*/
unsigned rotate_left(unsigned x, int n)
{
    int w = sizeof(int) << 3;
    // 1111 00...00
    int mask_left = (~0) << (w - n);
    // 0000 11...11
    int mask_right = ~mask_left;
    int left = (x & mask_left) >> (w - n);
    int right = (x & mask_right) << n;
    return left | right;
}
