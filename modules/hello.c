#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>


int init_module(void)
{
printk("<1>Hello Linux!!\n");
return 0;
}


void cleanup_module(void)
{
printk("<1> Bye Linux!!\n");
}

//module_init(hello_init);
//module_exit(hello_exit);



MODULE_AUTHOR("parul,2016");
MODULE_DESCRIPTION("DEMO MODULE");
MODULE_LICENSE("Dual BSD/GPL");


