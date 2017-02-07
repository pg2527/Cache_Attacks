#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>


static inline uint32_t get_cycle_count(void)
{
        uint32_t r = 0;
        asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
        return r;
}

//static void enable_cpu_counters(void* data);

int main(){
	
char s[21] ="This is a test of 3";

int fd;
int ret;
int data;
size_t time;
printf("Opening the character driver file\n");
fd=open("/dev/mydriver",O_RDWR);


if(fd<0)
	fprintf(stdout,"failed acquiring file descriptor return status %d\n",fd);
	fflush(stdout);
    printf("Flush the entire cache\n");
    ret = write (fd,s,21);
    ret = read(fd, s,100);
   // printf("A buffer would be read from the driver file\n");
   // enable_cpu_counters(&data);
    time = get_cycle_count();
   // printf("%zu\n", time);
if(ret<0)
	printf("read operation failed with return status %zu\n",ret);
printf("Succesful Operation\n");
close(fd);

return 0;	
}

