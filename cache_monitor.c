#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <asm/smp.h>
#include <asm/cacheflush.h>
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

#define ARMV7_CCNT      31  /* Cycle counter */
#define ENABLE_CNTR (1 << ARMV7_CCNT)
#define ARMV7_PMNC_E        (1 << 0) /* Enable all counters */
#define ARMV7_CNTENS_C (1 << ARMV7_CCNT)


#if !defined(__arm__)
#error Module can only be compiled on ARM machines.
#endif

extern long simple_strtol(const char *,char **,unsigned int);
void memory_exit(void);
int memory_init(void);
int memory_open(struct inode *inode, struct file *filp); 
int memory_release(struct inode *inode, struct file *filp); 
ssize_t memory_read(struct file *filp, char *buf, 
                    size_t count, loff_t *f_pos);
ssize_t memory_write( struct file *filp, char *buf,
                      size_t count, loff_t *f_pos);


/* Structure that declares the usual file */
/* access functions */
struct file_operations memory_fops = {
  read: memory_read,
  write: memory_write,
  open: memory_open,
  release: memory_release
};

/* Global variables of the driver */
/* Major number */
int memory_major = 69;
/* Buffer to store data */
char *memory_buffer;

int memory_open(struct inode *inode, struct file *filp) {
  /* Success */
    printk("[+]" KERN_INFO "[" DRVR_NAME "] Opening device !\n");
  return 0;
}

int memory_release(struct inode *inode, struct file *filp) {
  /* Success */
  return 0;
}

ssize_t memory_read(struct file *filp, char *buf, 
                    size_t count, loff_t *f_pos) { 
  /* Transfering data to user space */ 
  copy_to_user(buf,memory_buffer,1);

  /* Changing reading position as best suits */ 
  if (*f_pos == 0) { 
    *f_pos+=1; 
    return 1; 
  } else { 
    return 0; 
  }
}

ssize_t memory_write( struct file *filp, char *buf,
                      size_t count, loff_t *f_pos) {

  char *tmp;
  char *endptr;
  char ret;

  tmp=buf+count-1;
  copy_from_user(memory_buffer,tmp,1);
  ret = simple_strtol(memory_buffer, &endptr, 10);
  if(endptr == NULL)
  {
      printk("[+]" KERN_INFO "[" DRVR_NAME "] Failed to read an integer!\n");
  }
  else
  {
      if(ret == FLUSHALL)
      {
            printk("[+]" KERN_INFO "[" DRVR_NAME "] FLUSH CACHE ALL");
            flush_cache_all();
      }
      
  }
  return 1;
}

/* Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int mydriver_device_open(struct inode *inode, struct file *file)
{
    printk("[+]" KERN_INFO "[" DRVR_NAME "] opening mydriver device.\n");
    return 0;
}

static void enable_cpu_counters(void* data)
{
    printk("[+]" KERN_INFO "[" DRVR_NAME "] enabling user-mode PMU access on CPU #%d", smp_processor_id());
    /* Enable user-mode access to counters. */
    asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));
    /* Enable all counters */
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r"(ARMV7_PMNC_E));
    asm volatile("mcr p15, 0, %0, c9, c12, 1" : : "r" (ARMV7_CNTENS_C));
}

static void disable_cpu_counters(void* data)
{
printk("[+]" KERN_INFO "[" DRVR_NAME "] disabling user-mode PMU access on CPU #%d",
smp_processor_id());
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r" (0));
    asm volatile("mcr p15, 0, %0, c9, c12, 2" : : "r" (ARMV7_CNTENS_C));
    /* Disable user-mode access to counters. */
    asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(0));
}


int init_module(void)
{
    printk("[+]" KERN_INFO "[" DRVR_NAME "] Module initialisation\n");
    on_each_cpu(enable_cpu_counters, NULL, 1);

    int result = register_chrdev(memory_major, "mydriver", &memory_fops);
    if (result < 0) {
        printk(
          "[+]" KERN_INFO "[" DRVR_NAME "]: cannot obtain major number %d\n", memory_major);
        return result;
    }

      /* Allocating memory for the buffer */
      memory_buffer = kmalloc(1, GFP_KERNEL); 
      if (!memory_buffer) { 
        result = -ENOMEM;
        goto fail; 
      } 
      memset(memory_buffer, 0, 1);

      printk("[+]" KERN_INFO "[" DRVR_NAME "] Inserting memory module\n"); 
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

  printk("[+]" KERN_INFO "[" DRVR_NAME "] Removing module\n");

}


void cleanup_module(void)
{
    printk(KERN_INFO "[" DRVR_NAME "] Module unloading\n");
    on_each_cpu(disable_cpu_counters, NULL, 1);
    memory_exit();
}

MODULE_AUTHOR("razaina.");
MODULE_LICENSE("GPL");
MODULE_VERSION("1");
MODULE_DESCRIPTION("Enables user-mode access to ARMv7 PMU counters + Trigger the
flush_cache_all() function.");
