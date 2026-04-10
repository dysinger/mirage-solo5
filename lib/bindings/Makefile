.PHONY: all clean

TOPDIR ?= ../../../solo5
CONFIG_TARGET ?= solo5
CONFIG_AHV ?= 0

ifeq ($(CONFIG_AHV),1)
# AHV target - build ahv bindings
all: ahv/libmirage-solo5_bindings.a

ahv/libmirage-solo5_bindings.a: ahv/start.o ahv/console.o ahv/platform.o
	ar r $@ $^

ahv/start.o: ahv/start.c ahv/ahv_abi.h ahv/bindings.h
	cc -c -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv -Itenders/ahv -std=c11 -O2 -g -Wall $< -o $@

ahv/console.o: ahv/console.c ahv/ahv_abi.h
	cc -c -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv -std=c11 -O2 -g -Wall $< -o $@

ahv/platform.o: ahv/platform.c ahv/ahv_abi.h
	cc -c -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv -std=c11 -O2 -g -Wall $< -o $@

clean:
	rm -f ahv/*.o ahv/libmirage-solo5_bindings.a

else
# Default/solo5 target - build stub bindings
all: stub/libmirage-solo5_bindings.a

stub/libmirage-solo5_bindings.a: stub/main.o stub/clock_stubs.o stub/mm_stubs.o
	ar r $@ $^

stub/main.o: stub/main.c
	cc -c -std=c11 -O2 -g -Wall $< -o $@

stub/clock_stubs.o: stub/clock_stubs.c
	cc -c -std=c11 -O2 -g -Wall $< -o $@

stub/mm_stubs.o: stub/mm_stubs.c
	cc -c -std=c11 -O2 -g -Wall $< -o $@

clean:
	rm -f stub/*.o stub/libmirage-solo5_bindings.a
endif
