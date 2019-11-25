obj-m += reg.o
CURRENT_PATH := $(shell pwd)
LINUX_KERNEL := $(shell uname -r)
LINUX_KERNEL_PATH := /root/linux-4.18
all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
	gcc reguser.c -o app
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
	rm -rf app
