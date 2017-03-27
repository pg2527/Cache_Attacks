#include<stdio.h>
#include<inttypes.h>
#include<stdlib.h>
#include<string.h>

static inline uint32_t get_cycle_count(void)
{
        uint32_t r = 0;
        asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
        return r;
}

int main(){
uint32_t time;
time = get_cycle_count();
printf( "%" PRIu32 "\n",time);
return 0;
}

