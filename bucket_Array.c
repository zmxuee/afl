#include <stdio.h>

// 定义数组
static const unsigned char count_class_lookup8[256] = {
    [0] = 0,
    [1] = 1,
    [2] = 2,
    [3] = 4,
    [4 ... 7] = 8,
    [8 ... 15] = 16,
    [16 ... 31] = 32,
    [32 ... 127] = 64,
    [128 ... 255] = 128};

int main()
{
    // 遍历并输出数组中的每个元素
    for (int i = 0; i < 256; i++)
    {
        printf("count_class_lookup8[%d] = %u\n", i, count_class_lookup8[i]);
    }
    return 0;
}
