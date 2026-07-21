# Facilitator notes  (⚠️ contains spoilers — don't screen-share this file)

## The planted bug (Demo 3)

**File:** `src/core/stats.c`, function `stats_reset()`.

```c
void stats_reset(stats_t *s)
{
    s->min   = 0;            /* BUG: should be INT16_MAX */
    s->max   = INT16_MIN;
    ...
}
```

`min` is seeded to `0`. Every sensor reading is ~2500 (25.00 °C), i.e. always
`> 0`, so the `value < s->min` test in `stats_update()` is never true and `min`
stays `0` forever. `max` is seeded correctly (`INT16_MIN`) so it works — which
is what makes the bug a good teaching case: same idea, one initialiser wrong.

**The fix:**

```c
s->min = INT16_MAX;   /* needs <limits.h>, already included */
```

After the fix, `min` tracks the true minimum (~22.xx °C).

> There is intentionally **no comment** flagging the bug in the actual source,
> so attendees can find it themselves. The `/* BUG */` note above lives only in
> this facilitator file.

## Expected baseline output

```
=== Genesys Firmware Demo  (LM3S6965 / Cortex-M3 / QEMU) ===
modules: scheduler + sensor + stats + telemetry + cmd
type 'help' + Enter for console commands

[t=...] raw=25.11 min=0.00 max=25.11 mean=25.11 n=1
[t=...] raw=24.60 min=0.00 max=25.11 mean=24.85 n=2
...
```

Note `min=0.00` throughout — that's the symptom.

## After Demo 2 (filter added)

Frames should also show a `smooth=` field. The smoothed value should lag the
raw value and swing less. Exact wording depends on how the agent wired it in —
review the diff rather than expecting a specific string.

## Resetting between practice runs / between sessions

Because the AI edits files live, reset the repo before the next run:

```bash
git stash            # keep the edits aside, or
git checkout .       # discard all edits, back to the committed baseline
git stash clear      # (optional) drop stashed experiments
```

## Common gotchas & recovery

| Symptom | Cause / fix |
|---|---|
| No output after the banner | SysTick not ticking. Confirm your QEMU is recent (`qemu-system-arm --version`); the `lm3s6965evb` SysTick works on current builds. Rebuild clean: `make clean && make run`. |
| `arm-none-eabi-gcc: not found` | Terminal isn't inside the container. Reopen: F1 → "Dev Containers: Reopen in Container". |
| `Ctrl-C` won't quit QEMU | Use **Ctrl-A** then **X**. (Ctrl-C is passed to the guest.) |
| GDB can't connect | The `./scripts/debug.sh` terminal must be left running; run `./scripts/gdb.sh` in a *second* terminal. |
| Attendee typing does nothing | With `-nographic`, keystrokes go to the guest UART; type `help` + Enter. Focus must be on the QEMU terminal. |
| AI edits broke the build | `git checkout .` to restore, then retry with a tighter prompt. |

## Pre-flight (do this once before the room arrives)

1. `make clean && make run` → confirm banner + telemetry, `min=0.00`.
2. `./scripts/debug.sh` + `./scripts/gdb.sh` in two terminals → confirm the
   breakpoint hits and `print *s` works. `quit` GDB, Ctrl-A X the QEMU.
3. `git checkout .` to make sure the tree is clean for the live session.

## Timing flex

- Running long? Demo 2's **2b** and all bonus prompts are cuttable.
- Running short? Ask the room to predict the bug before opening GDB, or run the
  "ringbuffer review" bonus prompt.
