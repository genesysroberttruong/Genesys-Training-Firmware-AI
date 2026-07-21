#!/usr/bin/env bash
# TERMINAL 1: start QEMU halted, waiting for a debugger on tcp:1234.
# Then, in a second terminal, run:  ./scripts/gdb.sh
set -euo pipefail
cd "$(dirname "$0")/.."
make -s
echo "QEMU is halted and waiting for GDB on :1234  (run ./scripts/gdb.sh in another terminal)"
exec qemu-system-arm -M lm3s6965evb -nographic -kernel build/firmware.elf -S -gdb tcp::1234
