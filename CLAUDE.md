# Project guide for AI assistants

> This file is loaded automatically by Claude Code at the start of a session.
> It tells the assistant what this project is, how to build/run it, and the
> conventions to follow. A good `CLAUDE.md` is the single highest-leverage way
> to make an AI assistant productive on a codebase.

## What this is

Bare-metal firmware for a **TI LM3S6965 (ARM Cortex-M3)**, running under the
**QEMU `lm3s6965evb`** machine — no physical hardware. It samples a simulated
sensor and reports telemetry over a UART console, driven by a small
cooperative scheduler.

## Build / run / debug

```bash
make            # build build/firmware.elf
make run        # build + run under QEMU  (quit: Ctrl-A then X)
make debug      # run under QEMU halted, waiting for GDB on :1234
```

Debugging uses two terminals: `./scripts/debug.sh` then `./scripts/gdb.sh`.
GDB is `gdb-multiarch` (see `scripts/gdb-init.gdb`).

## Layout

- `src/startup.c` — vector table + C runtime bring-up
- `src/hal/` — hardware drivers (UART). Peripheral register access only.
- `src/core/` — hardware-independent logic (scheduler, ring buffer, sensor,
  stats, telemetry, command handler)
- `include/lm3s6965.h` — memory-mapped register definitions
- `linker/lm3s6965.ld` — memory map & sections
- `docs/datasheets/` — **read these before writing peripheral register code**

## Conventions (please follow when writing code)

- **C, freestanding.** No hosted assumptions beyond the small newlib subset
  already linked.
- **No floating point.** `%f` is intentionally not linked — use fixed-point
  integers (e.g. hundredths of a degree, `2537` == 25.37 °C).
- **No dynamic allocation** (`malloc`/`free`) in drivers or core modules; use
  static or caller-provided storage.
- **One module = one `.c` + one `.h`** in `src/core/` or `src/hal/`, with an
  include guard and a short file-header comment. State is `static` inside the
  `.c`. The `Makefile` globs `src/**/*.c`, so new files are picked up
  automatically — no Makefile edit needed.
- **Match the surrounding style**: 4-space indent, brace on its own line for
  functions, snake_case, `s_`-prefixed file-static variables.
- Peripheral registers come from `include/lm3s6965.h`; if you need a new one,
  add it there with the address/bit from `docs/datasheets/`.

## Guardrails

- Prefer **not** to modify `src/startup.c`, `linker/lm3s6965.ld`, or
  `src/syscalls.c` unless the task is specifically about the boot/link/runtime.
- Always cite `file:line` for claims about the code so I can verify.
- Build with `make` after changes; report warnings rather than hiding them.
