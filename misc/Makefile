#obj-m:=hello.o memory.o

#CFLAGS = -I/usr/src/linux-headers-3.13.0-53/include

# Run this Makefile as follows:
# (MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
#

KDIR= /lib/modules/$(shell uname -r)/build

PWD = /home/user/Documents/Thesis/

MAKE = make

obj-m := memory.o

all:
$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install depmod -a

clean:
rm -f *~ $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
