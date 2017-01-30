
#define _GNU_SOURCE

#include<stdio.h>
#include<libflush/libflush.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <inttypes.h>
#include <getopt.h>
#include <sched.h>
#include <string.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BIND_TO_CPU 0
#define BIND_THREAD_TO_CPU 1
#define MIN(a, b) ((a) > (b)) ? (b) : (a)
#define LENGTH(x) (sizeof(x)/sizeof((x)[0]))
#define _STR(x) #x
#define STR(x) _STR(x)

/*Flush+Reload attack using flush_read_hit and flush_reload_miss*/
   

static void flush_reload_hit(libflush_session_t* libflush_session, void*
    address, size_t runs, size_t* histogram, size_t histogram_size, size_t
    histogram_scale);
static void flush_reload_miss(libflush_session_t* libflush_session, void*
    address, size_t runs, size_t* histogram, size_t histogram_size, size_t
    histogram_scale);


int main(int argc, char* argv[]) {
  size_t cpu = BIND_TO_CPU;
  size_t thread_cpu = BIND_THREAD_TO_CPU;
  size_t number_of_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  size_t histogram_size = 300;
  size_t histogram_scale = 5;
  size_t histogram_entries = 50000;
 /* Bind to CPU */
  cpu = cpu % number_of_cpus;
  thread_cpu = thread_cpu % number_of_cpus;

  if (libflush_bind_to_cpu(cpu) == false) {
    fprintf(stderr, "Warning: Could not bind to CPU: %zu\n", cpu);
  }

  /* Initialize libflush */
  libflush_session_args_t args = { 0 };
  args.bind_to_cpu = thread_cpu;
  libflush_session_t* libflush_session;
  if (libflush_init(&libflush_session, NULL) == false) {
        return -1;
    }
//    int foo;
 //   void* address = &foo;
   // libflush_flush(libflush_session, address);
    //libflush_access(libflush_session, address);
   // libflush_access_memory(address);    
// Use libflush...
    //libflush_flush(libflush_session, address);
  //  int time1 = libflush_reload_address(libflush_session, address);
 
/* Allocate histograms */
  size_t* hit_histogram = calloc(histogram_size, sizeof(size_t));
  if (hit_histogram == NULL) {
    fprintf(stderr, "Error: Could not allocate memory for histogram.\n");
    return -1;
  }

  size_t* miss_histogram = calloc(histogram_size, sizeof(size_t));
  if (miss_histogram == NULL) {
    fprintf(stderr, "Error: Could not allocate memory for histogram.\n");
    return -1;
  }

  /* Map memory region */
/*#define MAP_SIZE 4096
  void* array = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (array == MAP_FAILED) {
    fprintf(stderr, "Error: Could not map memory.\n");

    free(hit_histogram);
    free(miss_histogram);

    return -1;
  }
*/    
/* Chose target address */
//  void* address = (void*) ((char*) array + MAP_SIZE / 2);
int foo;
void *address = &foo;  
libflush_access_memory(address);

/* Measure hit function */
flush_reload_hit(libflush_session, address, histogram_entries,
      hit_histogram, histogram_size, histogram_scale);

/* Measure miss function */
libflush_flush(libflush_session, address);
flush_reload_miss(libflush_session, address, histogram_entries,
      miss_histogram, histogram_size, histogram_scale);
 //   printf("The value of time in cpu cycles %d\n",time1);
 //   libflush_flush(libflush_session, address);
 //   int time2 = libflush_reload_address_and_flush(libflush_session,address);
//    printf("The value of time in cpu cycles after flushin %d\n",time2);
    // Terminate libflush
   // if (libflush_terminate(libflush_session) == false) {
   //     return -1;
   // }
unsigned int i;
for (i = 0; i < histogram_size; i++) {
    fprintf(stdout, "%4zu: %10zu %10zu\n", i * histogram_scale, hit_histogram[i], miss_histogram[i]);
//    if (logfile != NULL) {
//      fprintf(logfile, "%zu,%zu,%zu\n", i * histogram_scale, hit_histogram[i], miss_histogram[i]);
  //  }
  }

free(hit_histogram);
free(miss_histogram);
 if (libflush_terminate(libflush_session) == false) {
        return -1;
    }


    return 0;
}

static void
flush_reload_hit(libflush_session_t* libflush_session, void* address, size_t runs, size_t* histogram, size_t
    histogram_size, size_t histogram_scale)
{
  unsigned int i;
  for (i = 0; i < runs; i++) {
    size_t time = libflush_reload_address(libflush_session, address);
    histogram[MIN(histogram_size - 1, time / histogram_scale)]++;
    sched_yield();
  }
}

static void
flush_reload_miss(libflush_session_t* libflush_session, void* address, size_t runs, size_t* histogram, size_t
    histogram_size, size_t histogram_scale)
{
  unsigned int i;
  for (i = 0; i < runs; i++) {
    size_t time = libflush_reload_address_and_flush(libflush_session, address);
    histogram[MIN(histogram_size - 1, time / histogram_scale)]++;
    sched_yield();
  }
}

/*
static void
flush_flush_hit(libflush_session_t* libflush_session, void* address, size_t runs, size_t* histogram, size_t
    histogram_size, size_t histogram_scale)
{
  for (unsigned int i = 0; i < runs; i++) {
    libflush_reload_address(libflush_session, address);
    size_t time = libflush_flush_time(libflush_session, address);
    histogram[MIN(histogram_size - 1, time / histogram_scale)]++;
    sched_yield();
  }
}

static void
flush_flush_miss(libflush_session_t* libflush_session, void* address, size_t runs, size_t* histogram, size_t
    histogram_size, size_t histogram_scale)
{
  for (unsigned int i = 0; i < runs; i++) {
    size_t time = libflush_flush_time(libflush_session, address);
    histogram[MIN(histogram_size - 1, time / histogram_scale)]++;
    sched_yield();
  }
}
*/
