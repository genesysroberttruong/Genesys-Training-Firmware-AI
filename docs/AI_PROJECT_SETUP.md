# Setting up a project so AI is actually effective

*The opening segment of the workshop. The point: 10 minutes of project setup
makes every later AI interaction faster and more accurate. This repo is already
set up the "right way" — we'll tour the pieces, then use them in the demos.*

Give the model **context** (what the project is), **ground truth** (the
hardware docs), and **guardrails** (conventions + permissions). Firmware is
unforgiving: a hallucinated register address compiles fine and fails silently,
so grounding matters more here than in most software.

## The five that matter most

### 1. A `CLAUDE.md` at the repo root  →  see [../CLAUDE.md](../CLAUDE.md)
Loaded automatically at the start of every session. Put the things you'd tell a
new engineer on day one: what the project is, how to build/run/debug, the
directory layout, coding conventions, and guardrails ("don't touch the linker
script unless the task is about it"). This is the single highest-leverage file
— everything else builds on it.

### 2. A `docs/datasheets/` folder  →  see [datasheets/](datasheets/)
Drop the MCU reference manual, peripheral datasheets, and board schematic here.
Claude Code can read PDFs, but an extracted markdown table of the registers you
actually use (see [datasheets/lm3s6965-peripherals.md](datasheets/lm3s6965-peripherals.md))
is faster to cite and trivial to verify. Then you can say *"using the datasheet,
write the register access"* and get grounded, checkable code instead of a
plausible guess.

### 3. Coding conventions in `CLAUDE.md`
No floating point, no `malloc` in drivers, one module = `.c`+`.h`, fixed-point
units, naming. Stated once, the AI writes code that matches your codebase and
sails through review — you'll see this pay off live in the "write a module"
demo.

### 4. A permissions allowlist  →  see [../.claude/settings.json](../.claude/settings.json)
Pre-approve safe, routine commands (`make`, `qemu`, `gdb`, read-only `git`) so
the agent isn't stopping to ask on every build. Keeps a live session — and your
daily flow — moving, without handing over anything destructive.

### 5. Custom slash commands  →  see [../.claude/commands/](../.claude/commands/)
Turn a prompt you type often into a command. This repo ships `/explain-module`
and `/build`. Great for encoding team-standard workflows ("review like this",
"generate a driver from this template").

## Worth doing too (mention, don't demo)

- **Commit before you let the AI loose.** A clean git state makes every AI
  change a reviewable diff and a one-command undo (`git checkout .`).
- **Keep a known-good reference** (golden UART log, expected output) so you and
  the AI can compare against "correct" when debugging.
- **Give it a way to self-verify** — a `make` target, a host-buildable unit
  test. An AI that can run the build catches its own mistakes.
- **A driver/template to copy** — point at an existing module and say "write the
  new one like this." Examples beat descriptions.
- **`.gitattributes` for line endings** — on a Windows + Linux-container team,
  pin LF so scripts and Makefiles don't break (this repo does this).
- **Focus large PDFs** — tell the AI the section/page range instead of "read
  the whole 1,200-page manual."

## The one rule that doesn't change

Setup makes the AI faster and better grounded — it does **not** remove you from
the loop. You still review and verify every change, especially register-level
code. Grounding lowers the odds of a wrong answer; it never guarantees a right
one.
