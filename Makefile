# Makefile - build the LM3S6965 firmware with the ARM bare-metal toolchain.
#
#   make          build build/firmware.elf
#   make run      build, then run under QEMU (Ctrl-A then X to quit)
#   make debug    build, then run under QEMU halted, waiting for gdb on :1234
#   make clean    remove build artefacts

CROSS   ?= arm-none-eabi-
CC       = $(CROSS)gcc
OBJCOPY  = $(CROSS)objcopy
SIZE     = $(CROSS)size

CPU      = -mcpu=cortex-m3 -mthumb

# -O0 -g3 keeps the debug demo pleasant: every variable is live and
# source lines map one-to-one to instructions.
CFLAGS   = $(CPU) -O0 -g3 -Wall -Wextra \
           -ffreestanding -fno-common \
           -Iinclude -Isrc

LDFLAGS  = $(CPU) -T linker/lm3s6965.ld -nostartfiles \
           -Wl,--gc-sections

SRCS     = $(wildcard src/*.c src/hal/*.c src/core/*.c)
OBJS     = $(SRCS:.c=.o)

TARGET   = build/firmware.elf

QEMU     = qemu-system-arm
QEMUFLAGS = -M lm3s6965evb -nographic -kernel $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p build
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	$(OBJCOPY) -O binary $@ build/firmware.bin
	@echo "---"
	$(SIZE) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	$(QEMU) $(QEMUFLAGS)

debug: $(TARGET)
	$(QEMU) $(QEMUFLAGS) -S -gdb tcp::1234

# ----------------------------------------------------------------------------
# Host-side unit tests.
#
# The hardware-independent modules (ringbuffer, stats, sensor) are compiled
# and run with the NATIVE compiler - no ARM, no QEMU - so logic can be tested
# in milliseconds. HOSTCC is the host cc, not the cross-compiler.
# ----------------------------------------------------------------------------
HOSTCC    ?= cc
TEST_SRCS  = tests/test_main.c src/core/ringbuffer.c src/core/stats.c src/core/sensor.c

test:
	@mkdir -p build
	$(HOSTCC) -std=c11 -Wall -Wextra -Iinclude -Isrc $(TEST_SRCS) -o build/tests
	./build/tests

clean:
	rm -f $(OBJS) $(TARGET) build/firmware.bin build/tests

.PHONY: all run debug test clean
