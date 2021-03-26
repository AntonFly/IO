obj-m :=bl_driver.o
all :
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean :
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
do:
	sudo insmod bl_driver.ko
rm:
	sudo rmmod bl_driver.ko
