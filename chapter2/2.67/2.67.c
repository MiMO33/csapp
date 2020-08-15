#include<assert.h>

int int_size_is_32() 
{

        int x = 0;
        return 0;
}

int main(int args, char** argv) 
{
        #if __WORDSIZE == 32
                assert(0 == int_size_is_32());
        #endif
        return 0;
}