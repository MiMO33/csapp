/*
* 无法提取复数
*/

int xbyte(unsigned word, int bytenum)
{
    return (int) word << ((3 - bytenum) << 3) >> (3 << 3);
}