#!/usr/bin/env bash
# Build and run the firmware under QEMU.
# Quit QEMU with:  Ctrl-A  then  X
set -euo pipefail
cd "$(dirname "$0")/.."
make -s
exec qemu-system-arm -M lm3s6965evb -nographic -kernel build/firmware.elf
