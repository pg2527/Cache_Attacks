#define _GNU_SOURCE
#define __ARMCC_VERSION
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <sys/cachectl.h>
//#include <asm/cachectl.h>
#include <inttypes.h>
#include <string.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <time.h>
#include <libflush/libflush.h>
#include <linux/perf_event.h>

#define __ARM_ARCH_7a__ 1
#define N 4096

static inline void dcache_clean(void);
unsigned long long getticks(void);
static inline size_t get_cycle_count(void);
void clearcache(char* begin, char *end);
static inline void flush(void);
static uint32_t get_ccsidr();
unsigned char key[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

char* base;
char* probe;
char* end;
char *tmp;

int main(){
	//printf("the value is %zu\n",get_ccsidr());
int array[N];
//int steps =64*1024 * 1024; 
int steps = 64; 
int array_length= N-1;
int i , j;
struct timespec start_time = {0,0};
struct timespec start2_time = {0,0};
struct timespec end_time = {0,0};
struct timespec end2_time = {0,0};
unsigned char plaintext[] =
  {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
 // unsigned char plaintext[]="Aditya";
  unsigned char ciphertext[128];
  unsigned char restoredtext[128];
  
  
//  libflush_session_t* libflush_session;
  AES_KEY key_struct;
  AES_set_encrypt_key(key, 128, &key_struct);


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
   int dr_fd;
	ssize_t ret;
	char my_buf[2]="3";
	char new[3000];
	dr_fd = open( "/dev/mydriver", O_RDWR );
	if(dr_fd<0)
		printf("failed acquiring file descriptor return status %d\n",dr_fd);
/*Mapping the shared Library*/
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
  base = (char*) mmap(0, map_size,PROT_EXEC, MAP_SHARED, fd, 0);
  end = base + size;
  tmp = base + 0x14f45c;
  volatile unsigned int *p = (volatile unsigned int *)tmp;
  printf("The address from where library is mapped : %p\n",base);
getchar();
/*
for (i=1;i<=1024;i=i*2)
 {   // long long start_time = getticks();
	  
	 size_t start_time = get_cycle_count();
	   for( j=0;j<steps;j=j+i)
	  {
		  array[j]*=3;
	  }
	size_t end_time = get_cycle_count();
	//long long end_time = getticks();
	printf("%d: %zu\n", i,end_time-start_time);
}
// Arbitrary number of steps
*/
//long long start_time = getticks();
//size_t start_time = get_cycle_count();
size_t nano_sec=0 ;
size_t nano_sec2=0;
size_t sec =0;
size_t sec2 =0;
int sample;
char * tmp_new = tmp +64;
    //array[(i * 16) & array_length]++;
libflush_session_t* libflush_session;    
libflush_init(&libflush_session, NULL);
void *address = &array;


/*Warm-up round*/
AES_encrypt(plaintext,ciphertext,&key_struct);
//size_t time_pre_evict = libflush_reload_address(libflush_session,tmp);
/*First-round of AES*/
clock_gettime(CLOCK_MONOTONIC, &start_time);
AES_encrypt(plaintext,ciphertext,&key_struct);
clock_gettime(CLOCK_MONOTONIC, &end_time);
size_t time_pre_evict = libflush_reload_address(libflush_session,tmp_new);
/*Eviction of a cache-set*/
//size_t time_evict = libflush_reload_address_and_evict(libflush_session,tmp);
/*
ret=write(dr_fd,my_buf,2);
ret = read(fd,new,3);
	//printf("%s ,%d\n",my_buf,ret);
	if(ret<0)
		printf("read operation failed with return status %ld\n",ret);
	close(fd);
printf("Done with cache flushing\n");
*/
/*Performing final AES round*/
clock_gettime(CLOCK_MONOTONIC, &start2_time);
AES_encrypt(plaintext,ciphertext,&key_struct);
clock_gettime(CLOCK_MONOTONIC, &end2_time);

/*
for(j=1;j<(1024*1024);j++)
{for(i=0;i<array_length;i+=1)
  sample=array[i];}
  */
//dcache_clean();
//clock_gettime(CLOCK_MONOTONIC, &start2_time);
/*for(i=0;i<4;i++) 
     printf("%02x\n",*(tmp+3-i));
*/
size_t time_post_evict =libflush_reload_address(libflush_session,tmp_new);
for(i=0;i<4;i++) 
     printf("%02x\n",*(tmp_new+3-i));
//size_t time_post_evict = libflush_reload_address_and_evict(libflush_session,tmp);
//size_t time_post_and_evict = libflush_reload_address_and_evict(libflush_session,tmp);
//clock_gettime(CLOCK_MONOTONIC, &end2_time);

     // (x & lengthMod) is equal to (x % arr.Length)
 //   array[(i * 16) & array_length]++; // (x & lengthMod) is equal to (x % arr.Length);
//long long end_time = getticks();
//size_t end_time = get_cycle_count();

nano_sec = end_time.tv_nsec-start_time.tv_nsec;
nano_sec2 = end2_time.tv_nsec-start2_time.tv_nsec;
sec = end_time.tv_sec-start_time.tv_sec;
sec2 = end2_time.tv_sec-start2_time.tv_sec;
printf("Time taken to access address  pre-eviction is %zu  \n",time_pre_evict );
printf("Average nano-sec is %zu and sec is %zu\n" ,nano_sec,sec);
printf("Average nano-sec is %zu and sec is %zu\n" ,nano_sec2,sec2);
//getchar();
printf("Time taken to access address  is %zu  \n",time_post_evict );
//printf("time post evict is %zu and time post and evict is %zu \n" ,time_post_evict,time_post_and_evict );
libflush_terminate(libflush_session);
return 0;
}

static inline size_t get_cycle_count(void)
{
        size_t r = 0;
        __asm__ __volatile__("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
        return r;
}

void clearcache(char* begin, char *end)
{	
	const int syscall = 0x000f0002;
	__asm__ __volatile__ (
		"mov	 r0, %0\n"			
		"mov	 r1, %1\n"
		"mov	 r7, %2\n"
		"mov     r2, #0x0\n"
		"svc     0x00000000\n"
		:
		:	"r" (begin), "r" (end), "r" (syscall)
		:	"r0", "r1", "r7"
		);
	}
	
static inline void flush(void)
 {
	 int r = 0;
     __asm__ __volatile__("mcr p15, 0, %0, c7, c6, 1"::"r"(r)); 
      /*__asm__ __volatile__(    "mcr    p15, 0, %0, c7, c14, 0\n"
                 "       mcr     p15, 0, %0, c7, c10, 4"
                     :
                    : "r" (0)
                     : "cc");
                                                
 */
 } 
 
 
static inline void dcache_clean(void)
 {
     const int zero = 0;
     /* clean entire D cache -> push to external memory. */
     __asm__ __volatile__ ("1: mrc p15, 0, r15, c7, c10, 3\n"
                     " bne 1b\n" ::: "cc");
     /* drain the write buffer */
    __asm __volatile__ ("mcr p15, 0, %0, c7, c10, 4"::"r" (zero));
 }
 
 
 void __clear_cache(void *start, void *end) {
//    #define __ARM_NR_cacheflush 0x0f0002
         register int start_reg __asm("r0") = (int) (intptr_t) start;
         const register int end_reg __asm("r1") = (int) (intptr_t) end;
         const register int flags __asm("r2") = 0;
         const register int syscall_nr __asm("r7") = __ARM_NR_cacheflush;
         __asm __volatile("svc 0x0"
                          : "=r"(start_reg)
                          : "r"(syscall_nr), "r"(start_reg), "r"(end_reg),
                            "r"(flags));
         }


static uint32_t get_ccsidr(void)
{
	uint32_t ccsidr;
	__asm__ __volatile__("mrc p15,1,%0,c0,c0,0":"=r"(ccsidr));
	return ccsidr;
}
	
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


