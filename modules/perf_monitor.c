#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>


#define ARMV7_CCNT      31  /* Cycle counter */
#define ENABLE_CNTR (1 << ARMV7_CCNT)
#define ARMV7_PMNC_E        (1 << 0) /* Enable all counters */
#define ARMV7_CNTENS_C (1 << ARMV7_CCNT)

static void enable_cpu_counters(void* data)
{
   // printk("[+]" KERN_INFO "[" DRVR_NAME "] enabling user-mode PMU access on CPU #%d", smp_processor_id());
    /* Enable user-mode access to counters. */
    printk("Enabling PMU access on CPU %d",smp_processor_id());
    asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(1));
    /* Enable all counters */
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r"(ARMV7_PMNC_E));
    asm volatile("mcr p15, 0, %0, c9, c12, 1" : : "r" (ARMV7_CNTENS_C));
}

static void disable_cpu_counters(void* data)
{
//    printk(KERN_INFO "[" DRVR_NAME "] disabling user-mode PMU access on CPU #%d",smp_processor_id());
    /* Program PMU and disable all counters */
    printk("Disabling user-mode PMU access on CPU %d",smp_processor_id());
    asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r" (0));
    asm volatile("mcr p15, 0, %0, c9, c12, 2" : : "r" (ARMV7_CNTENS_C));
    /* Disable user-mode access to counters. */
    asm volatile("mcr p15, 0, %0, c9, c14, 0" :: "r"(0));
}

int init_module(void)
{
  //  printk("[+] " KERN_INFO "[" DRVR_NAME "] Module initialisation\n");
    printk("Module Initialisation");
    on_each_cpu(enable_cpu_counters, NULL, 1);
    return 0;
}

void cleanup_module(void)
{
   // printk("[+]" KERN_INFO "[" DRVR_NAME "] Module unloading\n");
    printk("Unloading module");
    on_each_cpu(disable_cpu_counters, NULL, 1);
}

MODULE_AUTHOR("razaina.");
MODULE_LICENSE("GPL");
MODULE_VERSION("1");
MODULE_DESCRIPTION("Enables user-mode access to ARMv7 PMU counters");
