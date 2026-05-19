obj-m += hello_module.o
obj-m += module_info.o
obj-m += module_param.o
obj-m += usage_provider.o
obj-m += usage_consumer.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
