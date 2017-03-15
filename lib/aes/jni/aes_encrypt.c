
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <inttypes.h>
#include <sched.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <openssl/aes.h>
#include <libflush/libflush.h>  
#include <sys/types.h>
#include <sys/ipc.h>
#include <linux/ashmem.h>
#include <time.h>

#define CPU_SETSIZE 1024
#define __NCPUBITS (8*sizeof(unsigned long))
typedef struct {
  unsigned long __bits[CPU_SETSIZE /__NCPUBITS];
}cpu_set_t;

#define BIND_TO_CPU 0 
static inline size_t get_cycle_count(void);
 
static const unsigned char key[] = {0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00};
char* base;
char* probe;
char* end;
unsigned char* tmp;
unsigned char* tmp_end;
struct timespec time2= {0,0};

struct timespec time1= {0,0};
int main()
 {
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
  base = (char*) mmap(0, map_size, PROT_READ, MAP_SHARED, fd, 0);
  end = base + size;
  tmp = 0xb6dda000 + 0x14f45c;
  tmp_end = tmp + 0x1000;
  //volatile unsigned int *p = (volatile unsigned int *)tmp;
  printf("The end value of tables are is %p\n",tmp_end);
  int i =0;
  for(i=0;i<4;i++) 
     printf("%02x\n",*(tmp+3-i));
 /*
  printf("Printing from next table\n");   
  for(i=0;i<4;i++) 
    printf("%02x\n",*(tmp_end+3-i));
   */
 
     
  unsigned char plaintext[] =
  {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
	unsigned char text[]="Aditya";
    unsigned char enc_out[128];
    unsigned char dec_out[128];
   // int i=0;
   
   /*
    
    int i =0;
    cpu_set_t set;
    int cpu = BIND_TO_CPU;
    */
    libflush_session_t* libflush_session;
    libflush_session_args_t args = { 0 };
	if (libflush_init(&libflush_session, &args) == false) {
    fprintf(stderr, "Error: Could not initialize libflush\n");
    return -1;
	}
	
	struct timespec start_time={0,0};
    struct timespec end_time ={0,0};
    AES_KEY enc_key, dec_key;
    
    AES_set_encrypt_key(key, 128, &enc_key);
    AES_encrypt(text, enc_out, &enc_key);  

    size_t result = 0;
    i=0;
    while(i!=50000){
		
   // size_t time = get_cycle_count();
     
     clock_gettime(CLOCK_MONOTONIC,&time2);
     AES_encrypt(text, enc_out, &enc_key); 
     clock_gettime(CLOCK_MONOTONIC,&time1);
  //   double measured_start_time = ((double)start_time.tv_sec + 1.0e-9*start_time.tv_nsec);
  //   double measured_end_time = ((double)end_time.tv_sec + 1.0e-9*end_time.tv_nsec);
 //   printf("The timing of AES is %.5f\n",measured_end_time-measured_start_time);
    //result = result + final_time;
    int j;
    for(j=0;*(enc_out+j)!=0x00;j++)
        printf("%X ",*(enc_out+j));
    fprintf(stdout, "\nThe time taken for AES is%ld\n",time1.tv_nsec-time2.tv_nsec);
    i++;
    }
    size_t average_time =(result)/10000;
    fprintf(stdout, "\nThe average computation time is:\t%zu \n",average_time);
    AES_set_decrypt_key(key,128,&dec_key);
    AES_decrypt(enc_out, dec_out, &dec_key);
    getchar();
    printf("original:\t");
    for(i=0;*(text+i)!=0x00;i++)
        printf("%X ",*(text+i));
    printf("\nencrypted:\t");
    for(i=0;*(enc_out+i)!=0x00;i++)
        printf("%X ",*(enc_out+i));
    printf("\ndecrypted:\t");
    for(i=0;*(dec_out+i)!=0x00;i++)
        printf("%X ",*(dec_out+i));
    printf("\n");

    return 0;
  }
  
  static inline size_t get_cycle_count(void)
{
        uint32_t r = 0;
         __asm__ __volatile__("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
        return r;
}
