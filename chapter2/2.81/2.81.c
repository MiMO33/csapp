#include<assert.h>      /*assert*/
#include<stdlib.h>      /*printf*/

int bit_wk(int k) 
{
        int ret = (~0) << k;
        return ret;
}

int bit_wjk(int j, int k)
{
        //0^w-k-j 1^k+j         
        int x = ~(~0 << (k + j));
        //0^w-k-j 1^k 0^j = 0^w-k-j 1^k+j - 0^w+k 1^j
        int ret = x - ~(~0 << j);
        return ret;
}

int main(int args, char** argv)
{
        assert(0xff000000 == bit_wk(56));
        assert(0x0000ff00 == bit_wjk( 8, 8));
        return 0;
}