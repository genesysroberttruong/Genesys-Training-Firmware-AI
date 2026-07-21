#!/usr/bin/env bash
# TERMINAL 2: connect GDB to the waiting QEMU instance (see debug.sh).
set -euo pipefail
cd "$(dirname "$0")/.."
exec gdb-multiarch -q -x scripts/gdb-init.gdb build/firmware.elf
