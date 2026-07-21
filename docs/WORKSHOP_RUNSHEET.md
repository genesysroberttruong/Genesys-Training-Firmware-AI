# 45-minute run sheet — AI for firmware development

**Goal:** show three concrete ways AI (Claude Code) speeds up firmware work:
understanding unfamiliar code, writing new modules, and debugging.

**Format:** live-driven by the facilitator on the projector; attendees follow
along in their own dev container. Exact prompts are in
[PROMPTS.md](PROMPTS.md) — keep it open in a second window.

> Timings assume everyone finished [ATTENDEE_PREP.md](ATTENDEE_PREP.md) and can
> already `make run`. If not, pair them up rather than debugging installs live.

---

## 0:00 – 0:02 — Welcome & baseline (2 min)

- One sentence on the target: *"Real firmware pattern — vector table, a HAL, a
  scheduler with an interrupt, drivers — but running in QEMU so nobody needs
  hardware."*
- Everyone runs it once so they see a known-good baseline:

  ```bash
  make run
  ```

  Point out the telemetry line and the fact that `min=0.00` looks suspicious —
  *"hold that thought, we'll come back to it."* (This is the planted bug.)
- Quit with **Ctrl-A then X**.

---

## 0:02 – 0:08 — Set your project up so AI is effective (6 min)

**Message:** *10 minutes of setup makes every later AI interaction faster and
more accurate. This repo is already set up the right way — here's what and why.*
Full reference: [AI_PROJECT_SETUP.md](AI_PROJECT_SETUP.md).

Tour these files on the projector (don't just describe them — open them):

1. **`CLAUDE.md`** (repo root) — auto-loaded context: what the project is,
   build/run commands, layout, conventions, guardrails. *The* highest-leverage file.
2. **`docs/datasheets/`** — MCU manual + an extracted register table so the AI
   grounds register code in the real hardware instead of hallucinating
   addresses. Firmware's biggest AI failure mode, fixed with a folder.
3. **Conventions inside `CLAUDE.md`** — no float, no malloc, one module =
   `.c`+`.h`. You'll watch these pay off in Demo 2.
4. **`.claude/settings.json`** — a permissions allowlist so `make`/`qemu`/`gdb`
   don't prompt every time (keeps a live demo moving).
5. **`.claude/commands/`** — custom slash commands (`/explain-module`, `/build`)
   that package a repeated prompt.
6. **`make test` + `tests/`** — host-native unit tests the AI can run to
   self-verify logic in milliseconds (no ARM/QEMU). Run it once so they see it
   green: `make test`.
7. **`examples/driver_template.c`** — a "write like this" reference so generated
   drivers match the house structure.

*Mention, don't demo:* commit before letting AI loose (clean diffs + easy
undo), keep a known-good reference output. Land the rule: **setup grounds the
AI; it doesn't replace your review.**

---

## 0:08 – 0:19 — Demo 1: Understand a complex codebase (11 min)

**Message:** *You dropped into an unfamiliar firmware repo. Instead of reading
12 files cold, have the agent map it for you.*

1. Open Claude Code. Run **Prompt 1a** ("explain this codebase / draw the
   architecture"). Let it read the tree and summarise:
   - the boot path (vector table → `Reset_Handler` → `main`),
   - the scheduler + the SysTick **interrupt**,
   - the data flow sensor → stats → telemetry → UART.
2. Run **Prompt 1b** — a pointed question it must answer from the code, e.g.
   *"How does a received console byte travel from the UART to a command being
   executed?"* This shows it tracing across `uart.c` → `ringbuffer.c` →
   `cmd.c`.
3. **Callback to setup:** run the custom command `/explain-module scheduler` —
   the same question, packaged as a one-word command from the setup segment.
4. **Talking points while it works:**
   - It reasons over the *whole* repo, not one open file.
   - Notice it cites `docs/datasheets/` for the register-level bits — that's the
     grounding from setup paying off.
   - Ask it for a diagram — great for onboarding docs.
   - Trust-but-verify: click the `file:line` references it cites.

*Wrap:* "In two minutes we got an architecture overview that would've taken 20
minutes of reading."

---

## 0:19 – 0:32 — Demo 2: Write a new module (13 min)

**Message:** *Now we extend it — the everyday firmware task of adding a small
library/driver and wiring it in.*

**Task:** add an exponential moving-average (EWMA) smoothing filter as its own
module and show a `smooth=` field in each telemetry frame.

1. Run **Prompt 2a**: ask Claude Code to create `src/core/filter.c` + `filter.h`
   (integer EWMA, no floating point, matching the house style) and to wire it
   into `telemetry.c`.
2. Review the diff *with the room* — this is the teaching moment:
   - Does it match the existing style (fixed-point, `static` state, headers)?
   - Did it add the source to the build? (The `Makefile` globs `src/core/*.c`,
     so a new file is picked up automatically — point that out.)
3. Rebuild and run:

   ```bash
   make run
   ```

   The frame now includes a `smooth=` value that lags/steadies the raw reading.
4. If time allows, run **Prompt 2b**: ask it to add a test for the filter in
   `tests/` and run `make test` — the agent **self-verifies** its own code.
   (Call back to the "give it a way to verify" setup tip.)

**Talking points:** you still own the review; the agent handles boilerplate
(header guards, wiring, formatting) so you focus on the algorithm.

---

## 0:32 – 0:43 — Demo 3: Debug with AI + GDB (11 min)

**Message:** *Back to that `min=0.00`. Let's find and fix a real bug the way
you would on the job — reproduce, hypothesise, inspect with a debugger, fix.*

1. Re-run `make run`, highlight the symptom: `min` is stuck at `0.00` even
   though every reading is ~25 °C. Ask the room *"what's wrong with that?"*
2. Run **Prompt 3a**: paste the symptom and ask Claude Code to find the likely
   cause. It should zero in on `stats_reset()` / `stats_update()` in
   `src/core/stats.c`.
3. **Confirm it with the debugger** (don't just trust the model). Two terminals:

   ```bash
   ./scripts/debug.sh    # terminal 1 — QEMU halts, waits for GDB
   ./scripts/gdb.sh      # terminal 2 — breaks in stats_update()
   ```

   In GDB:

   ```
   continue
   print *s          # see min, max, sum, count
   print value       # the incoming sample (~2500)
   print s->min      # it's 0 — value is never < 0, so min never updates
   ```

   *This is the point of the demo:* the agent forms the hypothesis, the debugger
   proves it.
4. Run **Prompt 3b**: ask Claude Code to fix it. Review the one-line change
   (initialise `min` to `INT16_MAX`, not `0`). Quit GDB (`quit`) and QEMU
   (**Ctrl-A then X**), then:

   ```bash
   make run
   ```

   `min` now tracks the real minimum. Done.
5. **If time allows — lock it in with a test:** ask Claude Code to add a `min`
   assertion to `tests/test_main.c` and run `make test`. Point out that `min`
   was the *one* thing the suite didn't cover — which is exactly why the bug
   survived. Now it can't come back.

**Talking points:** AI narrows the search; the debugger is ground truth. Use
both — never merge an AI fix you haven't confirmed.

---

## 0:43 – 0:45 — Wrap-up (2 min)

- Recap the three moves: **understand → build → debug**, each faster with AI.
- The golden rule: **the human reviews and verifies every change.**
- Point to [PROMPTS.md](PROMPTS.md) so they can replay this solo.

---

## Facilitator cheat-sheet

| Need                        | Command                                   |
|-----------------------------|-------------------------------------------|
| Build                       | `make`                                    |
| Build + run                 | `make run`                                |
| Quit QEMU                   | **Ctrl-A** then **X**                     |
| Debug (start QEMU halted)   | `./scripts/debug.sh`                      |
| Debug (attach GDB)          | `./scripts/gdb.sh`                        |
| Reset the repo between runs | `git stash` (or `git checkout .`)         |

See [FACILITATOR_NOTES.md](FACILITATOR_NOTES.md) for the bug spoiler, expected
output, and recovery tips.
