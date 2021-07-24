/*
* return 的类型是 unsigned 永远大于零
*/

void copy_int(int val, void* buf, int maxbytes) {
  if (maxbytes >= (int) sizeof(val)) {
    memcpy(buf, (void*)&val, sizeof(val));
  }
}