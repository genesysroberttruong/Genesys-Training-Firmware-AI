# Dev container image for the Firmware AI Workshop.
#
# Ships the ARM bare-metal toolchain, QEMU system emulator, and a
# multi-architecture GDB so the whole build/run/debug loop works with no
# physical hardware and no host-side toolchain.
FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        gcc-arm-none-eabi \
        binutils-arm-none-eabi \
        libnewlib-arm-none-eabi \
        qemu-system-arm \
        gdb-multiarch \
        make \
        git \
 && rm -rf /var/lib/apt/lists/*
