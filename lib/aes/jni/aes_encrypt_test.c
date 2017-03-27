#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
//#include "cacheutils.h"
//#include <map>
//#include <vector>
//#include <iostream>
// this number varies on different systems
#define MIN_CACHE_MISS_CYCLES (110)
#define THRESHOLD (110)

// more encryptions show features more clearly
#define NUMBER_OF_ENCRYPTIONS (25000)
#define NUMBER_OF_ENCRYPTIONS2 (64)
//#include "TI_aes_128_encr_only.h"

//extern void aes_encrypt(unsigned char *state, unsigned char *key);

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
 
int main()
 {
    unsigned char text[]="Aditya";
    unsigned char enc_out[80];
    unsigned char dec_out[80];

    AES_KEY enc_key, dec_key;

    AES_set_encrypt_key(key, 128, &enc_key);
    AES_encrypt(text, enc_out, &enc_key);      

    AES_set_decrypt_key(key,128,&dec_key);
    AES_decrypt(enc_out, dec_out, &dec_key);

    int i;

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
