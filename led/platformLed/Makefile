KERNELDIR ?=/home/topeet/linux/iTop4412_Kernel_3.0
	obj-m := platform_led.o
PWD := $(shell pwd)
CC = arm-none-linux-gnueabi-gcc
default:
	make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- -C $(KERNELDIR) M=$(PWD) modules

clean:
	-rm *.mod.c *.o *.order *.symvers *.ko *.cmd
