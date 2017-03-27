#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <asm/smp.h>
#include <asm/cacheflush.h>
///#include <mm/cache-v7.s>
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h> /* kmalloc() */
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/version.h>

#define PERF_DEF_OPTS (1 | 16)
#define PERF_OPT_RESET_CYCLES (2 | 4)
#define procfs_name "mydriver" // /dev/mydriver
#define FLUSHALL 3

#define ARMV7_DCACHE_INVAL_ALL 1
#define ARMV7_DCACHE_CLEAN_INVAL_ALL 2
#define ARMV7_INVAL_RANGE 3
#define ARMV7_DCACHE_CLEAN_INVAL_RANGE 4
#define ARMV7_CCNT      31  /* Cycle counter */
#define ENABLE_CNTR (1 << ARMV7_CCNT)
#define ARMV7_PMNC_E        (1 << 0) /* Enable all counters */
#define ARMV7_CNTENS_C (1 << ARMV7_CCNT)


#if !defined(__arm__)
#error Module can only be compiled on ARM machines.
#endif

struct cpu_cache_fns v7_cache_fns;
extern long simple_strtol(const char *,char **,unsigned int);
void memory_exit(void);
int memory_init(void);
int memory_open(struct inode *inode, struct file *filp); 
int memory_release(struct inode *inode, struct file *filp); 
ssize_t memory_read(struct file *filp, char *buf,size_t count, loff_t *f_pos);
ssize_t memory_write( struct file *filp,const char *buf,size_t count, loff_t *f_pos);
//static int mydriver_device_open(struct inode *inode, struct file *file);


/* Structure that declares the usual file */
/* access functions */
struct file_operations memory_fops = {
  read: memory_read,
  open: memory_open,
  release: memory_release,
  write: memory_write,
};

/* Global variables of the driver */
/* Major number */
int memory_major = 69;
/* Buffer to store data */
char *memory_buffer;
int num=0;
int num2=0;

int memory_open(struct inode *inode, struct file *filp) {
  /* Success */
    printk(" <KERN_INFO> Opening device !\n");
  return 0;
}

int memory_release(struct inode *inode, struct file *filp) {
  /* Success */
  return 0;
}

ssize_t memory_read(struct file *filp, char *buf, 
                    size_t count, loff_t *f_pos) { 
  /* Transfering data to user space */ 
  num=copy_to_user(buf,memory_buffer,1);

  /* Changing reading position as best suits */ 
  if (*f_pos == 0) { 
    *f_pos+=1; 
    return 1; 
  } else { 
    return 0; 
  }
 return num;
}

ssize_t memory_write( struct file *filp, const char *buf,
                      size_t count, loff_t *f_pos) {

  //char *tmp;
  char *endptr;
  char ret;

  //tmp=buf+count-1;
  num2 =copy_from_user(memory_buffer,buf,1);
  ret = simple_strtol(memory_buffer, &endptr, 10);
  printk("KERN_INFO value sent by the user%d\n",ret);
  if(endptr == NULL)
  {
      printk(" KERN_INFO  Failed to read an integer!\n");
  }
  else
  {
      if(ret == FLUSHALL)
      {
            flush_cache_all();
	       // v7_cache_fns.flush_user_all();
	       //v7_cache_fns.flush_kern_all();
	 //  int r = 0;
    	//  asm volatile("mcr p15, 0, %0, c7, c6, 1"::"r"(0));
 	  printk("KERN_INFO  FLUSH CACHE ALL");
      }
      
  }
  return num2;
}

/* Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */

/*static int mydriver_device_open(struct inode *inode, struct file *file)
{
    printk(" KERN_INFO opening mydriver device.\n");
    return 0;
}*/

static void enable_cpu_counters(void* data)
{
    printk("KERN_INFO enabling user-mode PMU access on CPU #%d", smp_processor_id());
    /* Enable user-mode access to counters. */
    asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));
    /* Enable all counters */
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r"(ARMV7_PMNC_E));
    asm volatile("mcr p15, 0, %0, c9, c12, 1" : : "r" (ARMV7_CNTENS_C));
}
 
static void disable_cpu_counters(void* data)
{
printk("KERN_INFO disabling user-mode PMU access on CPU #%d",smp_processor_id());
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r" (0));
    asm volatile("mcr p15, 0, %0, c9, c12, 2" : : "r" (ARMV7_CNTENS_C));
    /* Disable user-mode access to counters. */
    asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(0));
}


int init_module(void)
{
    int result;
    printk("KERN_INFO  Module initialisation\n");
    on_each_cpu(enable_cpu_counters, NULL, 1);
    result = register_chrdev(memory_major, "mydriver", &memory_fops);
    if (result < 0) {
        printk("KERN_INFO : cannot obtain major number %d\n", memory_major);
        return result;
    }

      /* Allocating memory for the buffer */
      memory_buffer = kmalloc(1, GFP_KERNEL); 
      if (!memory_buffer) { 
        result = -ENOMEM;
        goto fail; 
      } 
      memset(memory_buffer, 0, 1);

      printk("KERN_INFO  Inserting memory module\n"); 
      return 0;

      fail: 
        memory_exit(); 
        return result;
        return 0;
}

void memory_exit(void) {
  /* Freeing the major number */
  unregister_chrdev(memory_major, "mydriver");

  /* Freeing buffer memory */
  if (memory_buffer) {
    kfree(memory_buffer);
  }

  printk("KERN_INFO  Removing module\n");

}


void cleanup_module(void)
{
    printk("KERN_INFO Module unloading\n");
    on_each_cpu(disable_cpu_counters, NULL, 1);
    memory_exit();
}

MODULE_AUTHOR("razaina.");
MODULE_LICENSE("GPL");
MODULE_VERSION("1");
MODULE_DESCRIPTION("Enables user-mode access to ARMv7 PMU counters + Trigger the flush_cache_all() function.");
