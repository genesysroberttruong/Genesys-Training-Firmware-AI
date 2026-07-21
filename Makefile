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
           -Wl,--gc-sections -specs=nano.specs

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

clean:
	rm -f $(OBJS) $(TARGET) build/firmware.bin

.PHONY: all run debug clean
