#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>

unsigned long long getticks(void)
{
  static int fd,init = 0;
  static struct perf_event_attr attr;
  static unsigned long long buffer;

  if(!init) {
    attr.type = PERF_TYPE_HARDWARE;
    attr.config = PERF_COUNT_HW_CPU_CYCLES;
    fd = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0);
    if(fd < 0) {
      fprintf(stderr,"ERROR - Cannot open perf event file descriptor:\n");
      if(errno == -EPERM || errno == -EACCES)
        fprintf(stderr,"  Permission denied.\n");
      else if(errno == ENOENT)
        fprintf(stderr,"  PERF_COUNT_HW_CPU_CYCLES event is not supported.\n");
      else
        fprintf(stderr,"  Attempting to open the file descriptor returned %d (%s).\n",errno, strerror(errno));
      exit(-1);
    }
    init = 1;
  }
  read(fd,&buffer,sizeof(unsigned long long));
  return buffer;
}

int main(void)
{
  int i;
  printf("Preparing to test the perf event cycle counter.\n\n");
  printf("An \"illegal instruction\" error during this test indicates\n");
  printf("that the cycle counter is not available.\n\n");
  printf("If the perf event cycle counter works on your system, you should now see\n");
  printf("10 different cycle counts in the right hand column below.\n");
  for(i=1;i<=10;i++)
    printf(" %2i: %20Ld\n",i,getticks());
  printf("Perf event cycle counter read test complete.\n");
  printf("To use this counter in FFTW-NEON, use the configure option\n");
  printf("  --enable-perf-events\n");
}
