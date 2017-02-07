KERNEL_DIR=/usr/src/android/kernel
obj-m := cache_monitor.o
PWD := $(shell pwd)
all:
	$(MAKE) ARCH=arm CROSS_COMPILE=/home/user/Desktop/arm-eabi-4.6/bin/arm-eabi- -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
#	$(MAKE) ARCH=arm CROSS_COMPILE=/home/user/Desktop/toolchain/arm-eabi-4.8/bin/arm-eabi- -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) clean
