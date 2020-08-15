#include<assert.h>      /* assert*/
#include<limits.h>      /* INT_MIN*/

int tsub_ok(int x, int y)
{
        int res = 1;
        (y == INT_MIN) && (res = 0);

        int sub = x - y;
        int pos_over = (x > 0) && (y < 0) && (res > 0);
        int neg_over = (x < 0) && (y > 0) && (res < 0);

        return res && !(pos_over || neg_over);
}

int main()
{
        assert(0 == tsub_ok(0, INT_MIN));
        assert(0 != tsub_ok(0x00, 0x00));
        return 0;
}