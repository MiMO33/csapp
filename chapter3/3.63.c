long switch_prob(long x, long n) 
{
    long result = x;
    switch (n) {
    case 60:
    case 62:
        return 8 * x;
    case 63:
        return x >> 3;
    case 64:
        return x * 15;
    case 65:
        result = x * x;
    default:
        result = x + 0x4b;
    }
    return result
}