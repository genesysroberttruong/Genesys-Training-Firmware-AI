# Firmware AI Workshop

A small but realistic bare-metal firmware project used to teach **using AI
(Claude Code) for firmware development**. It targets an ARM Cortex-M3
(TI LM3S6965) and runs entirely inside **QEMU** — no hardware required.

Everything runs in a **VS Code Dev Container**, so every attendee gets an
identical toolchain on Windows with nothing to install beyond Docker + VS Code.

## What the firmware does

On boot it prints a banner, then a cooperative scheduler runs two tasks:

- **telemetry** — samples a simulated temperature sensor and prints a frame:
  `[t=...] raw=25.11 min=... max=... mean=... n=...`
- **cmd** — reads the console and handles `help`, `reset`, `stats`.

## Project layout

```
.devcontainer/     Dockerfile + devcontainer.json (the containerised toolchain)
include/           lm3s6965.h  - memory-mapped register definitions
linker/            lm3s6965.ld - memory map & sections
src/
  startup.c        vector table + C runtime bring-up
  syscalls.c       minimal newlib stubs
  log.c            printf-style logging over UART
  main.c           entry point: wires tasks into the scheduler
  hal/uart.c       polled UART0 console driver
  core/
    scheduler.c    tick-driven cooperative scheduler + SysTick ISR
    ringbuffer.c   power-of-two byte FIFO
    sensor.c       deterministic simulated sensor
    stats.c        running min / max / mean
    telemetry.c    the telemetry task
    cmd.c          console command handler
scripts/           run.sh, debug.sh, gdb.sh, gdb-init.gdb
docs/              workshop run sheet, prompts, prep & facilitator notes
Makefile           build / run / debug targets
```

## Quick start (inside the dev container)

```bash
make run       # build + run; quit QEMU with Ctrl-A then X
```

Debugging (two terminals):

```bash
./scripts/debug.sh    # terminal 1: QEMU halted, waiting for GDB
./scripts/gdb.sh      # terminal 2: connect GDB
```

## Workshop docs

- [docs/ATTENDEE_PREP.md](docs/ATTENDEE_PREP.md) — send this out beforehand.
- [docs/WORKSHOP_RUNSHEET.md](docs/WORKSHOP_RUNSHEET.md) — the 45-minute plan.
- [docs/PROMPTS.md](docs/PROMPTS.md) — copy-paste prompts for each demo.
- [docs/FACILITATOR_NOTES.md](docs/FACILITATOR_NOTES.md) — answers & recovery tips
  (**facilitator only — contains the debug-demo spoiler**).
