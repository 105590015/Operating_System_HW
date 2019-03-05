PWD            := $(shell pwd)
KERNEL_VERSION := $(shell uname -r)
KERNEL_DIR      = /usr/src/linux-headers-$(KERNEL_VERSION)/

MODULE_NAME  = hw1
obj-m := $(MODULE_NAME).o

all: hw1.c
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
