.PHONY: all clean

TOPDIR ?= ../../../solo5
CONFIG_TARGET ?= solo5
CONFIG_AHV ?= 0

SDK_PATH ?= /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk
CC_DARWIN ?= clang
CC_LINUX ?= clang --target=aarch64-linux-gnu

ifeq ($(CONFIG_AHV),1)
# AHV target - build ahv bindings for Linux/ELF (guest)
# Build using Linux cross-compiler to produce ELF format for AHV tender

all: ahv/libmirage-solo5_bindings.a

ahv/libmirage-solo5_bindings.a: ahv/start.o ahv/console.o ahv/platform.o ahv/block.o ahv/net.o ahv/time.o ahv/exit.o ahv/log.o ahv/mem.o ahv/cmdline.o ahv/cpu_aarch64.o ahv/mft.o ahv/printf.o ahv/lib.o ahv/tls.o ahv/abort.o ahv/crt.o
	$(AR) r $@ $^

ahv/start.o: ahv/start.c ahv/ahv_abi.h ahv/bindings.h crt_init.h version.h
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv -I$(TOPDIR)/bindings -Itenders/ahv ahv/start.c -o $@

ahv/console.o: ahv/console.c ahv/ahv_abi.h
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv ahv/console.c -o $@

ahv/platform.o: ahv/platform.c ahv/ahv_abi.h ahv/bindings.h
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv ahv/platform.c -o $@

ahv/block.o: ahv/block.c ahv/ahv_abi.h ahv/bindings.h mft_abi.h mft.h solo5.h
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv ahv/block.c -o $@

ahv/net.o: ahv/net.c ahv/ahv_abi.h ahv/bindings.h mft_abi.h mft.h solo5.h
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv ahv/net.c -o $@

ahv/time.o: ahv/time.c ahv/ahv_abi.h ahv/bindings.h cpu_aarch64.h
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv ahv/time.c -o $@

ahv/exit.o: ahv/exit.c ahv/bindings.h ahv/ahv_abi.h
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv ahv/exit.c -o $@

ahv/log.o: $(TOPDIR)/bindings/log.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv $(TOPDIR)/bindings/log.c -o $@

ahv/mem.o: ahv/mem.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv ahv/mem.c -o $@

ahv/cmdline.o: $(TOPDIR)/bindings/cmdline.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv $(TOPDIR)/bindings/cmdline.c -o $@

ahv/cpu_aarch64.o: ahv/cpu_aarch64.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv ahv/cpu_aarch64.c -o $@

ahv/mft.o: $(TOPDIR)/bindings/mft.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv $(TOPDIR)/bindings/mft.c -o $@

ahv/printf.o: $(TOPDIR)/bindings/printf.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv -D__SOLO5_BINDINGS__ $(TOPDIR)/bindings/printf.c -o $@

ahv/lib.o: $(TOPDIR)/bindings/lib.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv $(TOPDIR)/bindings/lib.c -o $@

ahv/tls.o: $(TOPDIR)/bindings/tls.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv $(TOPDIR)/bindings/tls.c -o $@

ahv/abort.o: $(TOPDIR)/bindings/abort.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv $(TOPDIR)/bindings/abort.c -o $@

ahv/crt.o: $(TOPDIR)/bindings/crt.c
	$(CC_LINUX) -c $(CFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/bindings -Iahv $(TOPDIR)/bindings/crt.c -o $@

clean:
	rm -f ahv/*.o ahv/libmirage-solo5_bindings.a

else
# Default/solo5 target - build stub bindings for macOS
all: stub/libmirage-solo5_bindings.a

CFLAGS_DARWIN := -std=gnu11 -O2 -g -Wall -target arm64-apple-darwin -Dlog=ahv_log -fno-stack-protector -include stddef.h -include stdint.h -include stdarg.h

stub/libmirage-solo5_bindings.a: stub/main.o stub/clock_stubs.o stub/mm_stubs.o
	$(AR) r $@ $^

stub/main.o: stub/main.c
	$(CC_DARWIN) -c -std=c11 -O2 -g -Wall $< -o $@

stub/clock_stubs.o: stub/clock_stubs.c
	$(CC_DARWIN) -c -std=c11 -O2 -g -Wall $< -o $@

stub/mm_stubs.o: stub/mm_stubs.c
	$(CC_DARWIN) -c -std=c11 -O2 -g -Wall $< -o $@

clean:
	rm -f stub/*.o stub/libmirage-solo5_bindings.a
endif