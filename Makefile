##
# Aspire300x Makefile
#

ifeq ($(KERNELVERSION),)
	KERNELVERSION=$(shell uname -r)
endif

MODNAME=aspire300x
TARGET=$(MODNAME).o

ifneq ($(KERNELRELEASE),)

	obj-m := $(TARGET)
	$(MODNAME)-objs := aspire3.o bl_control.o ec_control.o

else

	KERNELDIR ?= /lib/modules/$(KERNELVERSION)/build
	PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install: $(TARGET)
	install -D -m 644 $(MODNAME).ko /lib/modules/$(KERNELVERSION)/extra/$(MODNAME).ko
	/sbin/depmod -a

uninstall:
	modprobe -r $(MODNAME)
	rm -rv /lib/modules/$(KERNELVERSION)/extra/$(MODNAME).ko

load:
	insmod $(PWD)/$(MODNAME).ko

unload:
	rmmod $(MODNAME)

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif

