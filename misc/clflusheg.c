#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define ARRAYSIZE(arr) (sizeof(arr)/sizeof(arr[0]))


inline void
clflush(volatile void *p)
{
    asm volatile ("clflush (%0)" :: "r"(p));
}

inline uint64_t
rdtsc()
{
    unsigned long a, d;
    asm volatile ("cpuid; rdtsc" : "=a" (a), "=d" (d) : : "ebx", "ecx");
    return a | ((uint64_t)d << 32);
}

inline int func() { return 5;}

inline void test()
{
    uint64_t start, end;
    char c;
    start = rdtsc();
    func();
    end = rdtsc();
    printf("%ld ticks\n", end - start);
}

void flushFuncCache()
{
    // Assuming function to be not greater than 320 bytes.
    char* fPtr = (char*)func;
    clflush(fPtr);
    clflush(fPtr+64);
    clflush(fPtr+128);
    clflush(fPtr+192);
    clflush(fPtr+256);
}

int main(int ac, char **av)
{
    test();
    printf("Function must be cached by now!\n");
    test();
    flushFuncCache();
    printf("Function flushed from cache.\n");
    test();
    printf("Function must be cached again by now!\n");
    test();

    return 0;
}
