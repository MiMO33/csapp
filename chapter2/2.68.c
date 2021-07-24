/*
* Mask with least signficant n bits set to 1
* Examples: n = 6 --> 0x3F, n = 17 --> 0x1FFFF
* Assume 1 <= n <= w
*/
int lower_one_mask(int n)
{
    int w = sizeof(int) << 3;
    return ~((~0) << n);
}
