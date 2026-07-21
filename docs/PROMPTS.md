# Copy-paste prompts

Prompts to paste into **Claude Code** during the workshop. Adjust freely —
they're starting points, not a script. Keep this file open on a second monitor.

---

## Demo 1 — Understand the codebase

**1a — architecture overview**

```
This is an unfamiliar bare-metal firmware repo for an ARM Cortex-M3. Read the
source and give me:
1. A short plain-English summary of what the firmware does.
2. The boot sequence from reset to main().
3. An ASCII architecture diagram showing how the modules interact.
4. Where the interrupt(s) are and what they do.
Cite the file:line for each key claim so I can verify.
```

**1b — trace a specific data path**

```
Walk me through exactly how a byte typed at the console becomes an executed
command: which files and functions it passes through, in order, and where it
could be dropped.
```

---

## Demo 2 — Write a new module

**2a — create the filter module**

```
Add a new module `src/core/filter.c` and `src/core/filter.h` implementing an
integer exponential moving average (EWMA) suitable for the telemetry samples
(int16_t centi-degrees). No floating point. Match the existing code style:
header guards, static internal state, a small init/update API.

Then wire it into src/core/telemetry.c so each telemetry frame also prints a
`smooth=` field alongside `raw=`. Show me the diff and explain how the build
picks up the new file.
```

**2b — self-verify (optional)**

```
Add a test for the new filter module to tests/test_main.c: feed it a known
sequence and assert the smoothed output converges as expected. Then run
`make test` and show me the result.
```

---

## Demo 3 — Debug

**3a — find the bug**

```
When I run the firmware, every telemetry frame shows `min=0.00`, even though
every raw reading is around 25 °C. max and mean look correct. Find the most
likely cause in the code and explain your reasoning. Don't fix it yet.
```

**3b — fix the bug (after confirming in GDB)**

```
Confirmed in GDB: in stats_update(), `value < s->min` is never true because
s->min starts at 0 and all samples are positive. Fix stats_reset() so the
running minimum works correctly, and briefly explain why your fix is right.
```

**3c — lock it in with a test (optional)**

```
Add an assertion to the stats test in tests/test_main.c that would have caught
this bug (check `min` is the smallest value seen), then run `make test`.
```

---

## Bonus prompts (if you have time / for the recording)

```
Generate a README section documenting the telemetry frame format.
```

```
Review src/core/ringbuffer.c for correctness and concurrency issues, assuming
one producer in an ISR and one consumer in the main loop.
```

```
What would need to change to run this on a real STM32 instead of QEMU?
```
